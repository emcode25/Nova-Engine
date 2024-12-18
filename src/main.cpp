#include <Nova/nova.hpp>

#define SHADER_PATH(filename) ("../../../shaders/" ## filename)

Nova::Array<Nova::Entity> cubes;

Nova::Vector3 cubePositions[] = {
    { 0.0f,  0.0f,  0.0f},
    { 2.0f,  5.0f, -15.0f},
    {-1.5f, -2.2f, -2.5f},
    {-3.8f, -2.0f, -12.3f},
    { 2.4f, -0.4f, -3.5f},
    {-1.7f,  3.0f, -7.5f},
    { 1.3f, -2.0f, -2.5f},
    { 1.5f,  2.0f, -2.5f},
    { 1.5f,  0.2f, -1.5f},
    {-1.3f,  1.0f, -1.5f}
};

namespace Nova
{
    Nova::Float deltaTime = 0.0f;
    Nova::Float lastTime = 0.0f;
    
    GLFWwindow* window;
    
    Nova::Array<Nova::TextureInfo*> globalTextures;
    Nova::Array<Nova::MeshInfo> globalMeshes;
    
    Nova::Shader unlitShader;
    Nova::Shader forwardShader;
    Nova::Shader lightSourceShader;
    Nova::Shader activeObjShader;
    Nova::ShaderProgram activeProgram;

    Nova::Editor::EditorCamera editorCamera;

    Nova::World ecs;
    Nova::Entity activeObj;
    Nova::Array<Nova::Entity> entities;
    Nova::Lighting::LightManager lightManager;
    Nova::ShaderManager shaderManager;

    //Begins graphics specific setup for items like GLFW, GLAD, ImGUI, etc.
    Nova::Int initGraphics(void)
    {
        //---------------GLFW---------------//
        //Begin GLFW
        glfwInit();

        //Set hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Nova::CONST::OPENGL_VERSION_MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Nova::CONST::OPENGL_VERSION_MINOR);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        //Create and set window
        window = glfwCreateWindow(Nova::CONST::SCREEN_WIDTH, Nova::CONST::SCREEN_HEIGHT, "Nova Engine", NULL, NULL);
        if(window == NULL)
        {
            std::cerr << "Failed to create a GLFW window." << std::endl;
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);

        //Set up callbacks
        glfwSetFramebufferSizeCallback(window, Nova::resizeCB);
        glfwSetCursorPosCallback(window, mouseCB);


        //---------------GLAD---------------//
        //Initialize GLAD
        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cerr << "Failed to initialize GLAD." << std::endl;
            glfwTerminate();
            return -2;
        }


        //---------------OpenGL---------------//
        //Set the viewport
        glViewport(0, 0, Nova::CONST::SCREEN_WIDTH, Nova::CONST::SCREEN_HEIGHT);
        
        //Allow OpenGL settings
        glEnable(GL_DEPTH_TEST);

        //Increase lines thickness for highlighting objects
        glLineWidth(3.3f);

        //---------------IMGUI---------------//
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init();


        //---------------Nova---------------//
        //Create shader
        unlitShader.init(SHADER_PATH("vertex.vert"), SHADER_PATH("unlit.frag"));
        forwardShader.init(SHADER_PATH("vertex.vert"), SHADER_PATH("forward.frag"));
        lightSourceShader.init(SHADER_PATH("lights/vertex.vert"), SHADER_PATH("lights/fragment.frag"));
        activeObjShader.init(SHADER_PATH("active/vertex.vert"), SHADER_PATH("active/geometry.geom"), SHADER_PATH("active/fragment.frag"));

        shaderManager.addShader(unlitShader);
        shaderManager.addShader(forwardShader);
        shaderManager.addShader(lightSourceShader);
        shaderManager.addShader(activeObjShader);

        lightManager.addProgram(forwardShader.getProgram());

        stbi_set_flip_vertically_on_load(true);
        
        return 0;
    }

    Nova::Int initECS()
    {
        //Render system includes transformation information
        Nova::ecs.system<const Nova::Component::Transform, const Nova::Component::Mesh, const Nova::Component::PointLight>("Point Light Render")
            .run(PointLightRenderSystem);

        //Render system includes transformation information
        Nova::ecs.system<const Nova::Component::Transform, const Nova::Component::Mesh>("Object Render")
            .without<Nova::Component::PointLight>()
            .without<Nova::Component::DirectionalLight>() //TODO: Add more lighting types as needed
            .run(ObjectRenderSystem);

        //Load meshes
        loadCubeMesh();

        //Create test cubes
        cubes.resize(10);
        for (Nova::Int i = 0; i < 10; ++i)
        {
            Nova::Component::Transform transform;
            transform.position = cubePositions[i];
            transform.rotation = Nova::Vector3(0.8629051f, 0.2558994f, 0.4338701f).normalized() * 20.0f * i; 
            //TODO: Rotation about an arbitrary axis is broken
            transform.scale = Nova::Vector3(1.0f, 1.0f, 1.0f);

            Nova::String name = "Cube " + std::to_string(i);
            cubes[i] = Nova::createCube();
            cubes[i].set_doc_name(name.c_str());
            
            cubes[i].set<Nova::Component::Transform>(transform);

            entities.push_back(cubes[i]);
        }

        //Add a world camera (not the editor camera)
        auto cam = Nova::createCamera();
        cam.set_doc_name("Camera");
        entities.push_back(cam);


        //Add a light source
        Nova::Component::Transform plTransform = Nova::Component::DEFAULT_TRANSFORM;
        plTransform.position = Nova::Vector3(1.2f, 1.0f, 2.0f);
        plTransform.scale = Nova::Vector3(0.2f, 0.2f, 0.2f);

        Nova::BaseLight plBase;
        plBase.ambient  = Nova::Vector3(0.1f, 0.1f, 0.1f);
        plBase.diffuse  = Nova::Vector3(1.0f, 1.0f, 1.0f);
        plBase.specular = Nova::Vector3(0.0f, 0.0f, 0.0f);

        Nova::Component::PointLight plProps;
        plProps.base = plBase;
        plProps.constant  = 1.0f;
        plProps.linear    = 0.09f;
        plProps.quadratic = 0.032f;

        auto pointLight = Nova::createLightCube(plProps);
        pointLight.set<Nova::Component::Transform>(plTransform);
        pointLight.set_doc_name("Light 0");
        entities.push_back(pointLight);
        lightManager.addPointLight(pointLight);
        //TODO: Gather all point lights on game startup
        //TODO: See if flecs can do some kind of component initialization

        activeObj = cam;
        activeProgram = forwardShader.getProgram();

        return 0;
    }

    Nova::Int mainLoop()
    {
        while(!glfwWindowShouldClose(window))
        {
            //Calculate delta-times
            Nova::Float thisFrame = static_cast<Nova::Float>(glfwGetTime());
            Nova::deltaTime = thisFrame - Nova::lastTime;
            Nova::lastTime  = thisFrame;

            //Get the inputs and process them
            glfwPollEvents();
            processInput(Nova::window, Nova::editorCamera, Nova::deltaTime);

            //Let ImGUI work
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::ShowDemoWindow();

            Nova::EditorUI::MainMenu(Nova::window, Nova::ecs, Nova::entities);
            
            Nova::EditorUI::ShowObjectProperties(activeObj);
            Nova::EditorUI::ShowObjectList(Nova::entities, activeObj);

            ImGui::Render();

            //Clean the slate
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //Run the systems and pipelines
            ecs.progress(Nova::deltaTime);

            //Swap buffers
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(window);

            //Framerate lock at 60fps, TESTING ONLY
            Nova::Float remaining = static_cast<Nova::Float>(glfwGetTime()) - Nova::deltaTime;
            if (remaining < (1.0f / 60.0f))
            {
                std::this_thread::sleep_for(
                    std::chrono::nanoseconds(
                        static_cast<long long>(remaining * 1000000000.0f)));
            }
        }

        return 0;
    }

    void quit()
    {
        deleteTextures(globalTextures);

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(Nova::window);
        glfwTerminate();
    }
}

Nova::Int main(void)
{
    Nova::initGraphics();
    
    Nova::initECS();

    Nova::mainLoop();

    for (auto cube : cubes)
    {
        cube.destruct();
    }

    Nova::quit();

    return 0;
}