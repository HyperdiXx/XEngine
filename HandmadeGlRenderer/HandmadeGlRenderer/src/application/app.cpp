

#include <thread>
#include "app.h"

#include "../core/geometry/generator.h"
#include "../objects/skybox.h"
#include "../core/utility/log.h"

#include "../core/rendering/openglnew/renderer2d.h"
#include "../core/rendering/openglnew/batchrenderer.h"

#include "../core/rendering/sprite.h"
#include "../core/rendering/batchsprite.h"

#include "../core/rendering/ui/glui.h"

#include "../core/utility/clock.h"

#include "../core/rendering/texture.h"
#include "../core/rendering/pipeline/shader.h"

#include "../core/geometry/model.h"

#include "../core/rendering/text.h"
#include "../core/windowsystem/windowWin.h"

#include "../core/windowsystem/openglwnd.h"

#include "../objects/GBuffer.h"
#include "../core/rendering/api/opengl/glframebuffer.h"

#include "../core/rendering/scenes/scene.h"
#include "../core/rendering/pipeline/shadersBase.h"

#include "../core/geometry/assetmanager.h"

#include "../core/rendering/scenes/sceneObjects.h"
#include "../objects/terrain.h"

#include "../core/rendering/openglnew/forwardrender.h"

#include "../core/geometry/resources.h"
#include "../objects/lights/dirLight.h"


#include "../core/systems/textureload.h"

#include "../objects/shadowmap.h"

/*#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include "crtdbg.h"
#define DBG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DBG_NEW
#endif  */



#define BATCH 0

#ifdef _WIN64
namespace XEngine
{
    void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);


    XEngine::Camera camera;
    ForwardRender forwardRender;


    bool mouseStart = true;
    double lastX = WINDOWWIDTH / 2;
    double lastY = WINDOWHEIGHT / 2;

    static bool isUI = false;

    void Application::OpenGLScene1()
    {
/*#ifdef _DEBUG
        _CrtMemState _msBegin, _msEnd, _msDiff;
        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
        _CrtMemCheckpoint(&_msBegin);
#endif*/
        Rendering::WindowGL classicwindow("XEngine", WINDOWWIDTH, WINDOWHEIGHT);
        glfwSetCursorPosCallback(classicwindow.m_window, mouseCallback);
        glfwSetScrollCallback(classicwindow.m_window, scrollCallback);
        //glfwSetInputMode(classicwindow.m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        classicwindow.initStats();

        XEngine::GLGUI myUi(classicwindow.m_window, 1);

        GLFrameBuffer fbo(WINDOWWIDTH * 2, WINDOWHEIGHT * 2);

       
        Scene scene1("Scene1");

        Shader basicShader("src/shaders/basicShader.vs", "src/shaders/basicShader.fs");
        Shader loading("src/shaders/simplemodel.vs", "src/shaders/simplemodel.fs");

        Assets::Model secondmodel("src/models/nano/nanosuit.obj", false);
        Assets::Model grid("src/models/simple/plane.obj", false);

        ShaderBases &shdManager = ShaderBases::getInstance();

        shdManager.addShader("basic", &basicShader);
        shdManager.addShader("model", &loading);
   
        BasicMaterial testMat(shdManager.getShaderByName("model"));
        Transform testTransform;

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)WINDOWWIDTH / (float)WINDOWHEIGHT, 0.1f, 1000.0f);

        Entity mesh1(&grid, &testMat, &testTransform);
      
        scene1.addEntity(&mesh1);
       
        Shader lightShader("src/shaders/bloom.vs", "src/shaders/light.fs");
        Shader cubeMap("src/shaders/cubeMap.vs", "src/shaders/cubeMap.fs");
        Shader floorShader("src/shaders/lightPhongnew.vs", "src/shaders/lightPhongnew.fs");
        Shader textShader("src/shaders/text.vs", "src/shaders/text.fs");
        Shader lightMapShader("src/shaders/lightrender.vs", "src/shaders/lightrender.fs");
        Shader depthMapQuad("src/shaders/depth.vs", "src/shaders/depth.fs");
        Shader shadowShader("src/shaders/basicshadows.vs", "src/shaders/basicshadows.fs");
        Shader normalMappingShader("src/shaders/normalShader.vs", "src/shaders/normalShader.fs");

        Shader blurShader("src/shaders/blur.vs", "src/shaders/blur.fs");
        Shader bloomShader("src/shaders/bloom.vs", "src/shaders/bloom.fs");
        Shader mixedShader("src/shaders/blendingshader.vs", "src/shaders/blendingshader.fs");

        Shader dispShader("src/shaders/parallaxmapping.vs", "src/shaders/parallaxmapping.fs");

        Shader shaderGeometryPass("src/shaders/gBuffer.vs", "src/shaders/gBuffer.fs");
        Shader shaderLightingPass("src/shaders/defshading.vs", "src/shaders/defshading.fs");
        //Shader shaderLightBox = {}; ("8.1.deferred_light_box.vs", "8.1.deferred_light_box.fs");

        GeometryBuffer plane;
        std::vector<float> planeVertices = createPlane();

        createVertexBuffer(&plane, planeVertices);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

      /*  unsigned int diffuseMapTexture = XEngine::Utils::loadTexture("src/textures/desertsky_bk.tga");
        unsigned int specularMap = XEngine::Utils::loadTexture("src/textures/container2_specular.png");
        
        unsigned int floorTexture = XEngine::Utils::loadTexture("src/textures/get.png");

        unsigned int diffuseMapForNormals = XEngine::Utils::loadTexture("src/textures/bricks2.jpg");
        unsigned int normalMap = XEngine::Utils::loadTexture("src/textures/bricks2_normal.jpg");
        unsigned int dispMap = XEngine::Utils::loadTexture("src/textures/bricks2_disp.jpg");*/

        //unsigned int nanosuuitalbedo = loadTexture("src/textures/arm_dif.png");
        //unsigned int nanosuuitalbedo2 = loadTexture("src/textures/arm_dif.png");

        XEngine::Rendering::Skybox sky;
        sky.createSkybox();


        dispShader.enableShader();
        dispShader.setInt("diffuseMap", 0);
        dispShader.setInt("normalMap", 1);
        dispShader.setInt("depthMap", 2);

        shaderGeometryPass.enableShader();
        shaderGeometryPass.setInt("tex1", 0);
        shaderGeometryPass.setInt("tex2", 1);

        cubeMap.enableShader();
        cubeMap.setInt("skybox", 0);

        floorShader.enableShader();
        floorShader.setInt("floorTexture", 0);

        /*Win32UseShader(&normalMappingShader);
        setInt(&normalMappingShader, "diffuseMap", 0);
        setInt(&normalMappingShader, "normalMap", 1);

        Win32UseShader(&basicShader);
        setInt(&basicShader, "material.diffuse", 0);
        setInt(&basicShader, "material.specular", 1);


        Win32UseShader(&shadowShader);
        setInt(&shadowShader, "diffuseTexture", 0);
        setInt(&shadowShader, "shadowMap", 1);

        Win32UseShader(&depthMapQuad);
        setInt(&depthMapQuad, "depthMap", 0);

        Win32UseShader(&bloomShader);
        setInt(&bloomShader, "diffuseMap", 0);
        Win32UseShader(&blurShader);
        setInt(&blurShader, "diffuseTexture", 0);
        Win32UseShader(&mixedShader);
        setInt(&mixedShader, "scene", 0);
        setInt(&mixedShader, "bloomBlur", 1)*/


        //Assets::Model firstmodel("src/models/barrels/barrels.fbx", false);
        //Assets::Model cityModel("src/models/house/house2.obj", false);
        //Assets::Model sponza("src/models/sponza/sponza.obj", false);
        //Assets::Model room("src/models/room/fireplace_room.obj", false);
        //Assets::Model castlelow("src/models/rungholt/rungholt.obj", false);


        real64 deltaTime = 0.0;
        real64 lastFrame = 0.0;
        
        glm::mat4 floormodel = glm::mat4(1.0f);
        glm::mat4 model = glm::mat4(1.0f);

        glm::vec3 lightPos(0.0f, 3.0f, 3.0f);
        glm::vec3 lightposfloor(0.0f, 4.0f, 0.0f);

        glm::vec3 point = glm::vec3(rand() % 20, 0.0, rand() % 20);

        bool bloom = true;
        bool bloomKeyPressed = false;
        float exposure = 1.0f;

        glm::mat4 mvp = glm::mat4(1.0f);
        float nearp = 1.0f, farp = 7.5f;
        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearp, farp);
        glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        glm::mat4 lightspaceMatrix = lightProjection * lightView;

        Clock clockm, time;
        float ctime = 0;
        unsigned int frames = 0;
        int f = 0;

        XEngine::Rendering::Font text1("src/fonts/arial.ttf", &textShader);

        glm::mat4 orho = glm::mat4(1.0f);

        orho = glm::ortho(0.0f, float(WINDOWWIDTH), 0.0f, float(WINDOWHEIGHT), -1.0f, 1.0f);
        glm::mat4 view = glm::mat4(1.0f);
       
        Shader shadersprite("src/shaders/basic2d.vs", "src/shaders/basic2d.fs");

        glm::mat4 modelforsprite = glm::mat4(1.0f);
        modelforsprite = glm::translate(modelforsprite, glm::vec3(0.0f, 10.0f, 0.0f));

        shadersprite.enableShader();
        shadersprite.setMat4("projection", orho);

        glm::vec4 spriteColor = glm::vec4(1.0, 0.0, 0.0, 1.0);
        glm::vec4 spriteColor2 = glm::vec4(1.0, 1.0, 0.0, 1.0);
        glm::vec4 spriteColor3 = glm::vec4(0.0, 1.0, 0.0, 1.0);

        using namespace Rendering;

#if BATCH
        BatchSprite testsprite(100, 100, 40, 40, spriteColor);
        BatchSprite testsprite2(500, 500, 100, 100, spriteColor2);
        BatchRenderer2d renderer;
#else
        Sprite testsprite(10, 10, 20, 20, spriteColor, shadersprite);
        Sprite testsprite2(30, 10, 20, 20, spriteColor2, shadersprite);
        Sprite testsprite3(50, 10, 20, 20, spriteColor3, shadersprite);
        Renderer2d renderer;
#endif

      

        while (!classicwindow.isClosed())
        {
            LOG("\rUpdateLoop...");

            camera.speed = 10.0 * deltaTime;

            real64 currFrame = glfwGetTime();

            deltaTime = currFrame - lastFrame;
            lastFrame = currFrame;

            XEngine::processInput(classicwindow.m_window, &camera, isUI);

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



            //std::thread inp(XEngine::processInput, wb.window, &cam);
            //inp.detach();
            //inp.join();
     
            if (isUI == true)
            {
                
                myUi.startUpdate();
                glfwSetInputMode(classicwindow.m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                glfwSetCursorPosCallback(classicwindow.m_window, NULL);
            }
            else
            {
                glfwSetCursorPosCallback(classicwindow.m_window, mouseCallback);
                glfwSetInputMode(classicwindow.m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);                
            }

          
            shdManager.getShaderByName("model")->enableShader();
            view = camera.getViewMatrix();

            glm::mat4 viewproj = projection * view;
            
            shdManager.getShaderByName("model")->setMat4("viewproj", viewproj);
            shdManager.getShaderByName("model")->setVec3("lightPos", lightPos);
            shdManager.getShaderByName("model")->setVec3("camPos", camera.camPos);


            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, -2.0f, -2.0f));
            model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
            //mesh1.transf->setTranslation(glm::vec3(0.0f, -2.0f, -2.0f));
            //mesh1.transf->setScale(glm::vec3(0.2f, 0.2f, 0.2f));
            //loading.setMat4("model", model);
            //castlelow.drawMesh(&loading);
       
            mesh1.material->getShader()->setMat4("model", model);
            
            scene1.drawScene();


            shadersprite.enableShader();
            shadersprite.setVec4("color", spriteColor);
#if BATCH
            renderer.start();
#endif
            renderer.submit(&testsprite);
            renderer.submit(&testsprite2);
            renderer.submit(&testsprite3);
            

#if BATCH
            renderer.end();
#endif
            renderer.flush();

            sky.renderSkybox(&cubeMap, view, projection);

            if (isUI)
            {
                myUi.update(spriteColor);
            }
             
            text1.updateText("FPS: " + std::to_string(f), 10.0f, 700.0f, 0.3f, glm::vec3(1.0f, 1.0f, 1.0f));

            classicwindow.update();

            ++frames;

            if (time.elapsed() - ctime > 1.0f)
            {
                
                f = frames;
                frames = 0;
                ctime += 1.0f;
            }
        }

        delGeometry(&plane);

        myUi.shutdown();

        glfwTerminate();
        /*
#ifdef _DEBUG
        _CrtMemCheckpoint(&_msEnd);
        if (_CrtMemDifference(&_msDiff, &_msBegin, &_msEnd) > 0) {
            _CrtDumpMemoryLeaks();
            std::system("pause");
        }
#endif*/
    }

    
    void Application::OpenGLScene2()
    {
        Rendering::WindowGL classicwindow("XEngine", WINDOWWIDTH, WINDOWHEIGHT);
        glfwSetCursorPosCallback(classicwindow.m_window, mouseCallback);
        glfwSetScrollCallback(classicwindow.m_window, scrollCallback);
        
        classicwindow.initStats();
        
        XEngine::GLGUI myUi(classicwindow.m_window, 1);

       

        //unsigned int diffuseT= Texture2D::loadTexture2D("src/textures/wood.png");
        //unsigned int containerT = Texture2D::loadTexture2D("src/textures/container2.png");

       /* unsigned int hdrFBO;
        unsigned int buffers[2];*/

        /*glGenFramebuffers(1, &hdrFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
        glGenTextures(2, buffers);
        for (int i = 0; i < 2; ++i)
        {
            glBindTexture(GL_TEXTURE_2D, buffers[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WINDOWWIDTH, WINDOWHEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, buffers[i], 0);
        }

        unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        glDrawBuffers(2, attachments);*/

        Shader blurShader("src/shaders/blur.vs", "src/shaders/blur.fs");
        Shader bloomShader("src/shaders/bloom.vs", "src/shaders/bloom.fs");
        Shader mixedShader("src/shaders/blendingshader.vs", "src/shaders/blendingshader.fs");
       
  
        Shader skyboxShader("src/shaders/cubeMap.vs", "src/shaders/cubeMap.fs");

        
        float skyboxVertices[] = 
        {
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };

        unsigned int skyboxVAO, skyboxVBO;
        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);
        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        std::vector<std::string> textyures;
        
        textyures.push_back("src/textures/right.jpg");
        textyures.push_back("src/textures/left.jpg");
        textyures.push_back("src/textures/top.jpg");
        textyures.push_back("src/textures/bottom.jpg");
        textyures.push_back("src/textures/front.jpg");
        textyures.push_back("src/textures/back.jpg");
        
        //unsigned int cubemapTexture = Utils::loadCubemap(textyures);

        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, nrChannels;
        for (unsigned int i = 0; i < textyures.size(); i++)
        {
            unsigned char *data = stbi_load(textyures[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Cubemap texture failed to load at path: " << textyures[i] << std::endl;
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        skyboxShader.enableShader();
        skyboxShader.setInt("skybox", 0);
    
        while (!classicwindow.isClosed())
        {
            LOG("\rUpdate loop...");

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glDepthFunc(GL_LEQUAL); 
            skyboxShader.enableShader();
            glm::mat4 model = glm::mat4(1.0f);
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WINDOWWIDTH / (float)WINDOWHEIGHT, 0.1f, 100.0f);

            glm::mat4 view = glm::mat4(glm::mat3(camera.getViewMatrix()));
            skyboxShader.setMat4("view", view);
            skyboxShader.setMat4("projection", projection);

            glBindVertexArray(skyboxVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
            glDepthFunc(GL_LESS);

            
            classicwindow.update();
        }


        myUi.shutdown();

        glfwTerminate();
    }

    void Application::OpenGLScene3()
    {
        Rendering::WindowGL classicwindow("XEngine", WINDOWWIDTH, WINDOWHEIGHT);
        glfwSetCursorPosCallback(classicwindow.m_window, mouseCallback);
        glfwSetScrollCallback(classicwindow.m_window, scrollCallback);

        //classicwindow.initStats();

        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);

        Shader shaderGeometryPass("src/shaders/gBuffer.vs", "src/shaders/gBuffer.fs");
        Shader shaderLightingPass("src/shaders/defshading.vs", "src/shaders/defshading.fs");
        Shader shaderLightBox("src/shaders/slight.vs", "src/shaders/slight.fs");

        shaderGeometryPass.setupShaderFile();
        shaderLightingPass.setupShaderFile();
        shaderLightBox.setupShaderFile();


        Assets::Model secondmodel("src/models/nano/nanosuit.obj", false);
        std::vector<glm::vec3> objectPositions;
        objectPositions.push_back(glm::vec3(-3.0, -3.0, -3.0));
        objectPositions.push_back(glm::vec3(0.0, -3.0, -3.0));
        objectPositions.push_back(glm::vec3(3.0, -3.0, -3.0));
        objectPositions.push_back(glm::vec3(-3.0, -3.0, 0.0));
        objectPositions.push_back(glm::vec3(0.0, -3.0, 0.0));
        objectPositions.push_back(glm::vec3(3.0, -3.0, 0.0));
        objectPositions.push_back(glm::vec3(-3.0, -3.0, 3.0));
        objectPositions.push_back(glm::vec3(0.0, -3.0, 3.0));
        objectPositions.push_back(glm::vec3(3.0, -3.0, 3.0));



        unsigned int gBuffer;
        glGenFramebuffers(1, &gBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        unsigned int gPosition, gNormal, gAlbedoSpec;

        glGenTextures(1, &gPosition);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WINDOWWIDTH, WINDOWHEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

        glGenTextures(1, &gNormal);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WINDOWWIDTH, WINDOWHEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

        glGenTextures(1, &gAlbedoSpec);
        glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WINDOWWIDTH, WINDOWHEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);

        unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
        glDrawBuffers(3, attachments);

        unsigned int rboDepth;
        glGenRenderbuffers(1, &rboDepth);
        glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WINDOWWIDTH, WINDOWHEIGHT);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        const unsigned int NR_LIGHTS = 32;
        std::vector<glm::vec3> lightPositions;
        std::vector<glm::vec3> lightColors;
        srand(13);
        for (unsigned int i = 0; i < NR_LIGHTS; i++)
        {        
            float xPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
            float yPos = ((rand() % 100) / 100.0) * 6.0 - 4.0;
            float zPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
            lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
         
            float rColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
            float gColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
            float bColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
            lightColors.push_back(glm::vec3(rColor, gColor, bColor));
        }

        shaderLightingPass.enableShader();
        shaderLightingPass.setInt("gPosition", 0);
        shaderLightingPass.setInt("gNormal", 1);
        shaderLightingPass.setInt("gAlbedoSpec", 2);


        real32 deltaTime = 0.0f;
        real32 lastFrame = 0.0f;


        while (!classicwindow.isClosed())
        {

            LOG("\rUpdateLoop...");

            camera.speed = 10.0 * deltaTime;

            real64 currFrame = glfwGetTime();

            deltaTime = currFrame - lastFrame;
            lastFrame = currFrame;

            XEngine::processInput(classicwindow.m_window, &camera, isUI);


            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            

            glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WINDOWWIDTH / (float)WINDOWHEIGHT, 0.1f, 100.0f);
            glm::mat4 view = camera.getViewMatrix();
            glm::mat4 model = glm::mat4(1.0f);
            shaderGeometryPass.enableShader();
            shaderGeometryPass.setMat4("projection", projection);
            shaderGeometryPass.setMat4("view", view);
            for (unsigned int i = 0; i < objectPositions.size(); i++)
            {
                model = glm::translate(model, objectPositions[i]);
                model = glm::scale(model, glm::vec3(0.25f));
                shaderGeometryPass.setMat4("model", model);
                secondmodel.drawMesh(&shaderGeometryPass);
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            shaderLightingPass.enableShader();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, gPosition);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, gNormal);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);

            for (unsigned int i = 0; i < lightPositions.size(); i++)
            {
                shaderLightingPass.setVec3("lights[" + std::to_string(i) + "].pos", lightPositions[i]);
                shaderLightingPass.setVec3("lights[" + std::to_string(i) + "].color", lightColors[i]);
               
                const float linear = 0.7;
                const float quadratic = 1.8;
                shaderLightingPass.setFloat("lights[" + std::to_string(i) + "].linear", linear);
                shaderLightingPass.setFloat("lights[" + std::to_string(i) + "].quadratic", quadratic);
            }
            shaderLightingPass.setVec3("camPos", camera.camPos);

            Geometry::renderQuad();

            glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); 
            
            glBlitFramebuffer(0, 0, WINDOWWIDTH, WINDOWHEIGHT, 0, 0, WINDOWWIDTH, WINDOWHEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            shaderLightBox.enableShader();
            shaderLightBox.setMat4("projection", projection);
            shaderLightBox.setMat4("view", view);
            for (unsigned int i = 0; i < lightPositions.size(); i++)
            {
                model = glm::translate(model, lightPositions[i]);
                model = glm::scale(model, glm::vec3(0.125f));
                shaderLightBox.setMat4("model", model);
                shaderLightBox.setVec3("lightColor", lightColors[i]);
                Geometry::renderCube();
            }
                                  
            classicwindow.update();
        }


        glfwTerminate();
    }

    void Application::OpenGLScene4()
    {
        Rendering::WindowGL classicwindow("XEngine", WINDOWWIDTH, WINDOWHEIGHT);
        glfwSetCursorPosCallback(classicwindow.m_window, mouseCallback);
        glfwSetScrollCallback(classicwindow.m_window, scrollCallback);

        classicwindow.initStats();

        Scene pbrScene("pbrScene");

        XEngine::GLGUI myUi(classicwindow.m_window, 1);

        glEnable(GL_DEPTH_TEST);

        Camera camera;

        Texture2D albedo;
        albedo.loadFromFile("src/textures/pbr/pbr1/albedo.png", COLOR);
        Texture2D normal;
        normal.loadFromFile("src/textures/pbr/pbr1/normal.png", COLOR);
        Texture2D metallic;
        metallic.loadFromFile("src/textures/pbr/pbr1/metallic.png", COLOR);
        Texture2D roughness;
        roughness.loadFromFile("src/textures/pbr/pbr1/roughness.png", COLOR);
        Texture2D ao;
        ao.loadFromFile("src/textures/pbr/pbr1/ao.png", COLOR);

        real32 lastFrame = 0.0f;
        real32 deltaTime = 0.0f;

        Shader shader("src/shaders/pbr.vs", "src/shaders/pbr.fs");

        shader.setupShaderFile();

        shader.enableShader();
        shader.setInt("albedoMap", 0);
        shader.setInt("normalMap", 1);
        shader.setInt("metallicMap", 2);
        shader.setInt("roughnessMap", 3);
        shader.setInt("aoMap", 4);

        glm::vec3 lightPositions[] = {
            glm::vec3(0.0f, 0.0f, 10.0f),
        };
        glm::vec3 lightColors[] = {
            glm::vec3(150.0f, 150.0f, 150.0f),
        };

        int nrRows = 7;
        int nrColumns = 7;
        float spacing = 2.5;
        
        bool ui = false;

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WINDOWWIDTH / (float)WINDOWHEIGHT, 0.1f, 100.0f);
        shader.enableShader();
        shader.setMat4("projection", projection);
   
     
       // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
     
        while (!classicwindow.isClosed())
        {
            
            LOG("\rUpdateLoop...");

            camera.speed = 10.0 * deltaTime;

            real64 currFrame = glfwGetTime();

            deltaTime = currFrame - lastFrame;
            lastFrame = currFrame;

            XEngine::processInput(classicwindow.m_window, &camera, isUI);

            
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            if (isUI == true)
            {
                myUi.startUpdate();
                glfwSetInputMode(classicwindow.m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                glfwSetCursorPosCallback(classicwindow.m_window, NULL);

            }
            else
            {
                glfwSetCursorPosCallback(classicwindow.m_window, mouseCallback);
                glfwSetInputMode(classicwindow.m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }

            shader.enableShader();
            glm::mat4 view = camera.getViewMatrix();
            shader.setMat4("view", view);
            shader.setVec3("camPos", camera.camPos);

            albedo.bind(0);
            normal.bind(1);
            metallic.bind(2);
            roughness.bind(3);
            ao.bind(4);

            glm::mat4 model = glm::mat4(1.0f);
            for (int row = 0; row < nrRows; ++row)
            {
                for (int col = 0; col < nrColumns; ++col)
                {
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(
                        (float)(col - (nrColumns / 2)) * spacing,
                        (float)(row - (nrRows / 2)) * spacing,
                        0.0f
                    ));
                    shader.setMat4("model", model);
                    Geometry::renderSphere();
                }
            }

            for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
            {
                glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
                newPos = lightPositions[i];
                shader.setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
                shader.setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

                model = glm::mat4(1.0f);
                model = glm::translate(model, newPos);
                model = glm::scale(model, glm::vec3(0.5f));
                shader.setMat4("model", model);
                Geometry::renderSphere();
            }

            
            classicwindow.update();
        }
        
        myUi.shutdown();
        glfwTerminate();
    }

    void Application::OpenGLScene5()
    {
        using namespace Assets;

        Rendering::WindowGL classicWindow("XEngine", WINDOWWIDTH, WINDOWHEIGHT);

        glfwSetCursorPosCallback(classicWindow.m_window, mouseCallback);
        glfwSetScrollCallback(classicWindow.m_window, scrollCallback);
        
        //glfwSetInputMode(classicwindow.m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        classicWindow.initStats();

        XEngine::GLGUI myUi(classicWindow.m_window, 1);
      
        Resources resources;
        
        Assets::Model castle("src/models/castle/castle.obj", false);

        //Assets::Model *plane = Assets::AssetManager::getModel("Plane");
        //Assets::Model *secondmodel = Assets::AssetManager::getModel("Second");
        //Assets::Model *dragon = Assets::AssetManager::getModel("Erato");

        Scene scene1("Scene5");

        Shader screenShader("src/shaders/fbo.vs", "src/shaders/fbo.fs");
        Shader loading("src/shaders/simplemodel.vs", "src/shaders/simplemodel.fs");
        Shader basicLightModel("src/shaders/basicshadows.vs", "src/shaders/basicshadows.fs");
        Shader lightShader("src/shaders/simpleshader.vs", "src/shaders/simpleshader.fs");
        Shader cubeMap("src/shaders/cubeMap.vs", "src/shaders/cubeMap.fs");
        Shader textShader("src/shaders/text.vs", "src/shaders/text.fs");
        

        Texture2D planeText;
        planeText.loadFromFile("src/textures/logo.png", COLOR);

        ShaderBases &shdManager = ShaderBases::getInstance();

        shdManager.addShader("model", &loading);
        shdManager.addShader("screen", &screenShader);
        shdManager.addShader("cube", &lightShader);
        shdManager.addShader("shadowLight", &basicLightModel);

        BasicMaterial testMat(shdManager.getShaderByName("model"), &planeText);
        BasicMaterial testMat3(shdManager.getShaderByName("shadowLight"));
        BasicMaterial testMat2(shdManager.getShaderByName("screen"));
        Transform testTransform;

        SceneObjects &sceneSetup = SceneObjects::getInstance();

        sceneSetup.projMatrix = glm::perspective(glm::radians(45.0f), (float)WINDOWWIDTH / (float)WINDOWHEIGHT, 0.1f, 1000.0f);
        
        Entity mesh1(&castle, &testMat, &testTransform);
        //Entity mesh2(&castle, &testMat, &testTransform);

        scene1.addEntity(&mesh1);
        //scene1.addEntity(&mesh2);

        //Terrain::Terrain ter(120);

        GLFrameBuffer fb(WINDOWWIDTH, WINDOWHEIGHT);
        fb.init();


        //ShadowMap shadows(2048, 2048);

        glm::mat4 orho = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);


        orho = glm::ortho(0.0f, float(WINDOWWIDTH), 0.0f, float(WINDOWHEIGHT), -1.0f, 1.0f);
        
        Shader shadersprite("src/shaders/basic2d.vs", "src/shaders/basic2d.fs");

        glm::mat4 modelforsprite = glm::mat4(1.0f);
        modelforsprite = glm::translate(modelforsprite, glm::vec3(0.0f, 10.0f, 0.0f));

        shadersprite.enableShader();
        shadersprite.setMat4("projection", orho);

        glm::vec4 spriteColor = glm::vec4(1.0, 0.0, 0.0, 1.0);
        glm::vec4 spriteColor2 = glm::vec4(1.0, 1.0, 0.0, 1.0);
        glm::vec4 spriteColor3 = glm::vec4(0.0, 1.0, 0.0, 1.0);

        using namespace Rendering;

#if BATCH
        BatchSprite testsprite(100, 100, 40, 40, spriteColor);
        BatchSprite testsprite2(500, 500, 100, 100, spriteColor2);
        BatchRenderer2d renderer;
#else
        Sprite testsprite(50, 150, 50, 50, spriteColor, shadersprite);
        Sprite testsprite2(30, 10, 20, 20, spriteColor2, shadersprite);
        Sprite testsprite3(10, 400, 50, 50, spriteColor3, shadersprite);
        Renderer2d renderer;
#endif



        float quadVertices[] =
        {
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
             1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f,  1.0f,  0.0f, 1.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f,  1.0f, 1.0f
        };

        float quadTest[] = 
        {
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f,  1.0f, 1.0f
        };

        unsigned int ind[] = 
        {
            0, 1, 2,
            2, 3, 0,
        };

        unsigned int quadVAO, quadVBO;
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

        unsigned int ibo;

        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


        float vertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
        };
        
        unsigned int VBO, cubeVAO;
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindVertexArray(cubeVAO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        float skyboxVertices[] = 
        {      
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };

        unsigned int skyboxVAO, skyboxVBO;
        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);
        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        //XEngine::Rendering::Skybox sky;

        std::vector<std::string> mtextures;

        mtextures.push_back("src/textures/right.jpg");
        mtextures.push_back("src/textures/left.jpg");
        mtextures.push_back("src/textures/top.jpg");
        mtextures.push_back("src/textures/bottom.jpg");
        mtextures.push_back("src/textures/front.jpg");
        mtextures.push_back("src/textures/back.jpg");

        unsigned int cubemapTexture = Utils::loadCubemapTest(mtextures);

        //screenShader.enableShader();
        //screenShader.setInt("screenTexture", 0);
           
        lightShader.enableShader();
        lightShader.setInt("texture1", 0);

        cubeMap.enableShader();
        cubeMap.setInt("skybox", 0);

        Clock clockm, time;
        float ctime = 0;
        unsigned int frames = 0;
        int f = 0;

        XEngine::Rendering::Font text1("src/fonts/arial.ttf", &textShader);

        glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

        DirLight light(glm::vec3(0.0f, 3.0f, -10.0f), lightColor);
        DirLight lightFloor(glm::vec3(0.0f, 4.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
   
        glm::vec3 boxPos = glm::vec3(1.0f);


        bool isUi = false;
        bool isFrame = false;
        
        while (!classicWindow.isClosed())
        {
            forwardRender.getActiveCamera()->speed = 5.0f * sceneSetup.deltaTime;

            real64 currFrame = glfwGetTime();

            sceneSetup.deltaTime = currFrame - sceneSetup.lastFrame;
            sceneSetup.lastFrame = currFrame;

            XEngine::processInput(classicWindow.m_window, forwardRender.getActiveCamera(), isUI);

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            if (isUI)
            {
                myUi.startUpdate();
                glfwSetCursorPosCallback(classicWindow.m_window, NULL);
                glfwSetInputMode(classicWindow.m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
            else
            {
                glfwSetCursorPosCallback(classicWindow.m_window, mouseCallback);
                glfwSetInputMode(classicWindow.m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }

            if (isFrame)
            {
                fb.bind();
                glEnable(GL_DEPTH_TEST);
                glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            }


            //ter.up = 1.0f;
            //ter.render();

            shdManager.getShaderByName("model")->enableShader();
            view = forwardRender.getActiveCamera()->getViewMatrix();

            //light.setPos(sin(glfwGetTime()) * 3.0f, cos(glfwGetTime()) * 2.0f, 5.0 + cos(glfwGetTime())* 1.0f);

            glm::mat4 viewproj = sceneSetup.projMatrix * view;

            shdManager.getShaderByName("model")->setMat4("viewproj", viewproj);
            shdManager.getShaderByName("model")->setVec3("lightPos", light.getPos());
            shdManager.getShaderByName("model")->setVec3("camPos", forwardRender.getActiveCamera()->camPos);
            shdManager.getShaderByName("model")->setVec4("lightColor", lightColor);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, -2.0f, -2.0f));
            model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));

            glm::mat4 model2 = glm::mat4(1.0f);
            model2 = glm::translate(model, glm::vec3(0.0f, -2.0f, -2.0f));
            model2 = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            //mesh1.transf->setTranslation(glm::vec3(0.0f, -2.0f, -2.0f));
            //mesh1.transf->setScale(glm::vec3(0.2f, 0.2f, 0.2f));
            //loading.setMat4("model", model);
            //erato.drawMesh(&loading);

            mesh1.material->getShader()->setMat4("model", model);

            forwardRender.renderScene(&scene1);

            glm::mat4 mat3 = glm::mat4(1.0);

            boxPos.x = 1.0f + sin(glfwGetTime()) * 5.0f;
            boxPos.z = sin(glfwGetTime() / 2.0f) * 10.0f;

            mat3 = glm::translate(mat3, boxPos);


            lightShader.enableShader();

            planeText.bind(0);

            lightShader.setMat4("projection", sceneSetup.projMatrix);
            lightShader.setMat4("view", view);

            lightShader.setMat4("model", mat3);

            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);


            shadersprite.enableShader();
            shadersprite.setVec4("color", spriteColor);

#if BATCH
            renderer.start();
#endif
            renderer.submit(&testsprite);
            renderer.submit(&testsprite2);
            renderer.submit(&testsprite3);


#if BATCH
            renderer.end();
#endif
            renderer.flush();

            glm::vec3 lightPos = light.getPos();

            glDepthFunc(GL_LEQUAL);
            cubeMap.enableShader();
            view = glm::mat4(glm::mat3(view));
            cubeMap.setMat4("view", view);
            cubeMap.setMat4("projection", sceneSetup.projMatrix);
            cubeMap.setInt("skybox", 0);

            glBindVertexArray(skyboxVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);

            glDepthFunc(GL_LESS);

            text1.updateText("FPS: " + std::to_string(f), 10.0f, 700.0f, 0.3f, glm::vec3(0.0f, 0.0f, 0.0f));


            if (isFrame)
            {
                fb.unbind();
                glDisable(GL_DEPTH_TEST);
                glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                screenShader.enableShader();
                glBindVertexArray(quadVAO);
                glBindTexture(GL_TEXTURE_2D, fb.getColorTexture());
                glDrawArrays(GL_TRIANGLES, 0, 6);
                //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
                //planeText.bind(0);

                //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            }

            if (isUI)
            {
                myUi.setUIScene5(lightPos, lightColor, planeText.getHandle(), spriteColor, spriteColor2, spriteColor3, isFrame);
                light.setPos(lightPos);
                light.setColor(lightColor);
            }



            classicWindow.update();

            ++frames;

            if (time.elapsed() - ctime > 1.0f)
            {

                f = frames;
                frames = 0;
                ctime += 1.0f;
            }

        }

        glDeleteBuffers(1, &cubeVAO);
        glDeleteBuffers(1, &cubeVBO);
        glDeleteBuffers(1, &skyboxVAO);
        glDeleteBuffers(1, &skyboxVBO);
        
        myUi.shutdown();

        glfwTerminate();
       

    }


    void Application::terrainDemo()
    {
        Rendering::WindowGL classicWindow("XEngine", WINDOWWIDTH, WINDOWHEIGHT);

        glfwSetCursorPosCallback(classicWindow.m_window, mouseCallback);
        glfwSetScrollCallback(classicWindow.m_window, scrollCallback);
        glfwSetInputMode(classicWindow.m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        classicWindow.initStats();

        glEnable(GL_DEPTH_TEST);

        Scene scene1("Scene5");

        Shader screenShader("src/shaders/fbo.vs", "src/shaders/fbo.fs");
        Shader loading("src/shaders/simplemodel.vs", "src/shaders/simplemodel.fs");
        Shader lightShader("src/shaders/simpleshader.vs", "src/shaders/simpleshader.fs");

        //Assets::Model cube("src/models/simple/cube.obj", false);
        //Assets::Model sphere("src/models/simple/sphere.obj", false);
        Assets::Model plane("src/models/simple/plane.obj", false);
        Assets::Model secondmodel("src/models/nano/nanosuit.obj", false);

        Texture2D mapHeight;
        mapHeight.loadFromFile("src/textures/terrain/heightmap.png", COLOR);

        Texture2D planeText;
        planeText.loadFromFile("src/textures/container.jpg", COLOR);

        ShaderBases &shdManager = ShaderBases::getInstance();

        shdManager.addShader("model", &loading);
        shdManager.addShader("screen", &screenShader);
        //shdManager.addShader("cube", &lightShader);

        BasicMaterial testMat(shdManager.getShaderByName("model"), &planeText);
        BasicMaterial testMat2(shdManager.getShaderByName("screen"));
        Transform testTransform;

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)WINDOWWIDTH / (float)WINDOWHEIGHT, 0.1f, 1000.0f);

        Entity mesh1(&plane, &testMat, &testTransform);
        Entity mesh2(&secondmodel, &testMat, &testTransform);

        scene1.addEntity(&mesh1);
        scene1.addEntity(&mesh2);

        float vertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
        };
        // first, configure the cube's VAO (and VBO)
        unsigned int VBO, cubeVAO;
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindVertexArray(cubeVAO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);


        lightShader.enableShader();
        lightShader.setInt("texture1", 0);


        real32 deltaTime = 0.0f;
        real32 lastFrame = 0.0f;

        glm::vec3 lightPos(0.0f, 3.0f, 3.0f);
        glm::vec3 lightposfloor(0.0f, 4.0f, 0.0f);

        glm::mat4 view = glm::mat4(1.0f);

        bool isUi = false;

        while (!classicWindow.isClosed())
        {
            camera.speed = 10.0 * deltaTime;

            real64 currFrame = glfwGetTime();

            deltaTime = currFrame - lastFrame;
            lastFrame = currFrame;

            XEngine::processInput(classicWindow.m_window, &camera, isUI);

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


            shdManager.getShaderByName("model")->enableShader();
            view = camera.getViewMatrix();

            glm::mat4 viewproj = projection * view;

            shdManager.getShaderByName("model")->setMat4("viewproj", viewproj);
            shdManager.getShaderByName("model")->setVec3("lightPos", lightPos);
            shdManager.getShaderByName("model")->setVec3("camPos", camera.camPos);


            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, -2.0f, -2.0f));
            model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));

            glm::mat4 model2 = glm::mat4(1.0f);
            model2 = glm::translate(model, glm::vec3(0.0f, -2.0f, -2.0f));
            model2 = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
 


            mesh1.material->getShader()->setMat4("model", model);

            scene1.drawScene();

            glm::mat4 mat3 = glm::mat4(1.0);

            lightShader.enableShader();

            planeText.bind(0);

            lightShader.setMat4("projection", projection);
            lightShader.setMat4("view", view);

            lightShader.setMat4("model", mat3);

            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);


            classicWindow.update();
        }




        glfwTerminate();


    }

    void Application::pbr()
    {
       
        Rendering::WindowGL classicWindow("XEngine", WINDOWWIDTH, WINDOWHEIGHT);

        glfwSetCursorPosCallback(classicWindow.m_window, mouseCallback);
        glfwSetScrollCallback(classicWindow.m_window, scrollCallback);
        glfwSetInputMode(classicWindow.m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        //glfwSetInputMode(classicWindow.m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        classicWindow.initStats();


        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

        Shader pbrShader("src/shaders/pbr/pbr.vs", "src/shaders/pbr/pbr.fs");
        Shader equirectangularToCubemapShader("src/shaders/pbr/cubemap.vs", "src/shaders/pbr/equ_to_cubemap.fs");
        Shader irradianceShader("src/shaders/pbr/cubemap.vs", "src/shaders/pbr/irradiance.fs");
        Shader prefilterShader("src/shaders/pbr/cubemap.vs", "src/shaders/pbr/pref.fs");
        Shader brdfShader("src/shaders/pbr/brdf.vs", "src/shaders/pbr/brdf.fs");
        Shader backgroundShader("src/shaders/pbr/background.vs", "src/shaders/pbr/background.fs");

        pbrShader.enableShader();
        pbrShader.setInt("irradianceMap", 0);
        pbrShader.setInt("prefilterMap", 1);
        pbrShader.setInt("brdfLUT", 2);
        pbrShader.setInt("albedoMap", 3);
        pbrShader.setInt("normalMap", 4);
        pbrShader.setInt("metallicMap", 5);
        pbrShader.setInt("roughnessMap", 6);
        pbrShader.setInt("aoMap", 7);

        backgroundShader.enableShader();
        backgroundShader.setInt("environmentMap", 0);

        unsigned int ironAlbedoMap = Utils::loadTexture("src/textures/pbr/rusted_iron/albedo.png");
        unsigned int ironNormalMap = Utils::loadTexture("src/textures/pbr/rusted_iron/normal.png");
        unsigned int ironMetallicMap = Utils::loadTexture("src/textures/pbr/rusted_iron/metallic.png");
        unsigned int ironRoughnessMap = Utils::loadTexture("src/textures/pbr/rusted_iron/roughness.png");
        unsigned int ironAOMap = Utils::loadTexture("src/textures/pbr/rusted_iron/ao.png");

        unsigned int goldAlbedoMap = Utils::loadTexture("src/textures/pbr/gold/albedo.png");
        unsigned int goldNormalMap = Utils::loadTexture("src/textures/pbr/gold/normal.png");
        unsigned int goldMetallicMap = Utils::loadTexture("src/textures/pbr/gold/metallic.png");
        unsigned int goldRoughnessMap = Utils::loadTexture("src/textures/pbr/gold/roughness.png");
        unsigned int goldAOMap = Utils::loadTexture("src/textures/pbr/gold/ao.png");

        unsigned int grassAlbedoMap = Utils::loadTexture("src/textures/pbr/grass/albedo.png");
        unsigned int grassNormalMap = Utils::loadTexture("src/textures/pbr/grass/normal.png");
        unsigned int grassMetallicMap = Utils::loadTexture("src/textures/pbr/grass/metallic.png");
        unsigned int grassRoughnessMap = Utils::loadTexture("src/textures/pbr/grass/roughness.png");
        unsigned int grassAOMap = Utils::loadTexture("src/textures/pbr/grass/ao.png");

        unsigned int plasticAlbedoMap = Utils::loadTexture("src/textures/pbr/plastic/albedo.png");
        unsigned int plasticNormalMap = Utils::loadTexture("src/textures/pbr/plastic/normal.png");
        unsigned int plasticMetallicMap = Utils::loadTexture("src/textures/pbr/plastic/metallic.png");
        unsigned int plasticRoughnessMap = Utils::loadTexture("src/textures/pbr/plastic/roughness.png");
        unsigned int plasticAOMap = Utils::loadTexture("src/textures/pbr/plastic/ao.png");

        unsigned int wallAlbedoMap = Utils::loadTexture("src/textures/pbr/wall/albedo.png");
        unsigned int wallNormalMap = Utils::loadTexture("src/textures/pbr/wall/normal.png");
        unsigned int wallMetallicMap = Utils::loadTexture("src/textures/pbr/wall/metallic.png");
        unsigned int wallRoughnessMap = Utils::loadTexture("src/textures/pbr/wall/roughness.png");
        unsigned int wallAOMap = Utils::loadTexture("src/textures/pbr/wall/ao.png");

        glm::vec3 lightPositions[] = 
        {
            glm::vec3(-10.0f,  10.0f, 10.0f),
            glm::vec3(10.0f,  10.0f, 10.0f),
            glm::vec3(-10.0f, -10.0f, 10.0f),
            glm::vec3(10.0f, -10.0f, 10.0f),
        };

        glm::vec3 lightColors[] = 
        {
            glm::vec3(300.0f, 300.0f, 300.0f),
            glm::vec3(300.0f, 300.0f, 300.0f),
            glm::vec3(300.0f, 300.0f, 300.0f),
            glm::vec3(300.0f, 300.0f, 300.0f)
        };

        int nrRows = 7;
        int nrColumns = 7;
        float spacing = 2.5;

        unsigned int captureFBO;
        unsigned int captureRBO;
        glGenFramebuffers(1, &captureFBO);
        glGenRenderbuffers(1, &captureRBO);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

        stbi_set_flip_vertically_on_load(true);
        int width, height, nrComponents;
        float *data = stbi_loadf("src/textures/hdr/newport_loft.hdr", &width, &height, &nrComponents, 0);
        unsigned int hdrTexture;
        if (data)
        {
            glGenTextures(1, &hdrTexture);
            glBindTexture(GL_TEXTURE_2D, hdrTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data); // note how we specify the texture's data value to be float

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Failed to load HDR image." << std::endl;
        }
    
        unsigned int envCubemap;
        glGenTextures(1, &envCubemap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // enable pre-filter mipmap sampling (combatting visible dots artifact)
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        glm::mat4 captureViews[] =
        {
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
        };

      
        equirectangularToCubemapShader.enableShader();
        equirectangularToCubemapShader.setInt("equirectangularMap", 0);
        equirectangularToCubemapShader.setMat4("projection", captureProjection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, hdrTexture);

        glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        for (unsigned int i = 0; i < 6; ++i)
        {
            equirectangularToCubemapShader.setMat4("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            Geometry::renderCube();
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // then let OpenGL generate mipmaps from first mip face (combatting visible dots artifact)
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

        // pbr: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
        // --------------------------------------------------------------------------------
        unsigned int irradianceMap;
        glGenTextures(1, &irradianceMap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

        // pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
        // -----------------------------------------------------------------------------
        irradianceShader.enableShader();
        irradianceShader.setInt("environmentMap", 0);
        irradianceShader.setMat4("projection", captureProjection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

        glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        for (unsigned int i = 0; i < 6; ++i)
        {
            irradianceShader.setMat4("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            Geometry::renderCube();
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // pbr: create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
        // --------------------------------------------------------------------------------
        unsigned int prefilterMap;
        glGenTextures(1, &prefilterMap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // be sure to set minifcation filter to mip_linear 
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

        // pbr: run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
        // ----------------------------------------------------------------------------------------------------
        prefilterShader.enableShader();
        prefilterShader.setInt("environmentMap", 0);
        prefilterShader.setMat4("projection", captureProjection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        unsigned int maxMipLevels = 5;
        for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
        {
            // reisze framebuffer according to mip-level size.
            unsigned int mipWidth = 128 * std::pow(0.5, mip);
            unsigned int mipHeight = 128 * std::pow(0.5, mip);
            glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
            glViewport(0, 0, mipWidth, mipHeight);

            float roughness = (float)mip / (float)(maxMipLevels - 1);
            prefilterShader.setFloat("roughness", roughness);
            for (unsigned int i = 0; i < 6; ++i)
            {
                prefilterShader.setMat4("view", captureViews[i]);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                Geometry::renderCube();
            }
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        unsigned int brdfLUTTexture;
        glGenTextures(1, &brdfLUTTexture);

        glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

        glViewport(0, 0, 512, 512);
        brdfShader.enableShader();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Geometry::renderQuad();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WINDOWWIDTH / (float)WINDOWHEIGHT, 0.1f, 100.0f);
        pbrShader.enableShader();
        pbrShader.setMat4("projection", projection);
        backgroundShader.enableShader();
        backgroundShader.setMat4("projection", projection);

        real32 deltaTime = 0.0f;
        real32 lastFrame = 0.0f;

        bool isUi = false;

        int scrWidth, scrHeight;
        glfwGetFramebufferSize(classicWindow.m_window, &scrWidth, &scrHeight);
        glViewport(0, 0, scrWidth, scrHeight);

        while (!glfwWindowShouldClose(classicWindow.m_window))
        {
            camera.speed = 10.0 * deltaTime;

            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            processInput(classicWindow.m_window, &camera, isUi);

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            pbrShader.enableShader();
            glm::mat4 model = glm::mat4(1.0f);
            glm::mat4 view = camera.getViewMatrix();
            pbrShader.setMat4("view", view);
            pbrShader.setVec3("camPos", camera.camPos);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);

            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, ironAlbedoMap);
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, ironNormalMap);
            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_2D, ironMetallicMap);
            glActiveTexture(GL_TEXTURE6);
            glBindTexture(GL_TEXTURE_2D, ironRoughnessMap);
            glActiveTexture(GL_TEXTURE7);
            glBindTexture(GL_TEXTURE_2D, ironAOMap);

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-5.0, 0.0, 2.0));
            pbrShader.setMat4("model", model);
            Geometry::renderSphere();

            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, goldAlbedoMap);
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, goldNormalMap);
            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_2D, goldMetallicMap);
            glActiveTexture(GL_TEXTURE6);
            glBindTexture(GL_TEXTURE_2D, goldRoughnessMap);
            glActiveTexture(GL_TEXTURE7);
            glBindTexture(GL_TEXTURE_2D, goldAOMap);

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-3.0, 0.0, 2.0));
            pbrShader.setMat4("model", model);
            Geometry::renderSphere();

            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, grassAlbedoMap);
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, grassNormalMap);
            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_2D, grassMetallicMap);
            glActiveTexture(GL_TEXTURE6);
            glBindTexture(GL_TEXTURE_2D, grassRoughnessMap);
            glActiveTexture(GL_TEXTURE7);
            glBindTexture(GL_TEXTURE_2D, grassAOMap);

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-1.0, 0.0, 2.0));
            pbrShader.setMat4("model", model);
            Geometry::renderSphere();

            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, plasticAlbedoMap);
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, plasticNormalMap);
            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_2D, plasticMetallicMap);
            glActiveTexture(GL_TEXTURE6);
            glBindTexture(GL_TEXTURE_2D, plasticRoughnessMap);
            glActiveTexture(GL_TEXTURE7);
            glBindTexture(GL_TEXTURE_2D, plasticAOMap);

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(1.0, 0.0, 2.0));
            pbrShader.setMat4("model", model);
            Geometry::renderSphere();

            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, wallAlbedoMap);
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, wallNormalMap);
            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_2D, wallMetallicMap);
            glActiveTexture(GL_TEXTURE6);
            glBindTexture(GL_TEXTURE_2D, wallRoughnessMap);
            glActiveTexture(GL_TEXTURE7);
            glBindTexture(GL_TEXTURE_2D, wallAOMap);

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(3.0, 0.0, 2.0));
            pbrShader.setMat4("model", model);
            Geometry::renderSphere();

            // render light source (simply re-render sphere at light positions)
            // this looks a bit off as we use the same shader, but it'll make their positions obvious and 
            // keeps the codeprint small.
            for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
            {
                glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
                newPos = lightPositions[i];
                pbrShader.setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
                pbrShader.setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

                model = glm::mat4(1.0f);
                model = glm::translate(model, newPos);
                model = glm::scale(model, glm::vec3(0.5f));
                pbrShader.setMat4("model", model);
                Geometry::renderSphere();
            }

            // render skybox (render as last to prevent overdraw)
            backgroundShader.enableShader();

            backgroundShader.setMat4("view", view);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
            //glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap); // display irradiance map
            //glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap); // display prefilter map
            Geometry::renderCube();

            // render BRDF map to screen
            //brdfShader.enableShader();
            //Geometry::renderQuad();


            classicWindow.update();
        }

        glfwTerminate();

    }

    void Application::DX11InitEngine()
    {
        Rendering::WindowDX dxwnd;

        dxwnd.initEngine();

        LOG("XEngine v 0.0.1");

        dxwnd.update();
        dxwnd.shutdown();

    }


    void XEngine::framebufferSizeCallback(GLFWwindow* window, int32 width, int32 height)
    {
        glViewport(0, 0, width, height);
    }

    void mouseCallback(GLFWwindow* window, double xpos, double ypos)
    {
        if (mouseStart)
        {
            lastX = xpos;
            lastY = ypos;
            mouseStart = false;
        }

        double xoffset = xpos - lastX;
        double yoffset = lastY - ypos;

        lastX = xpos;
        lastY = ypos;

        forwardRender.getActiveCamera()->mouseMove(xoffset, yoffset);

    }

    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        camera.mouseScroll(yoffset);
    }





}
   

#endif

#if 0

model loading


/*view = glm::mat4(cam.getViewMatrix());

loading.Win32useShader();

loading.setMat4("projection", projection);
loading.setMat4("view", view);

glm::mat4 modelNanosuit = glm::mat4(1.0f);
modelNanosuit = glm::translate(modelNanosuit, glm::vec3(5.0f, -1.0f, 0.0f));
modelNanosuit = glm::scale(modelNanosuit, glm::vec3(0.2f, 0.2f, 0.2f));
loading.setMat4("model", modelNanosuit);
secondmodel.drawMesh(&loading);

modelNanosuit = glm::mat4(1.0f);
modelNanosuit = glm::translate(modelNanosuit, glm::vec3(10.0f, -1.0f, 0.0f));
modelNanosuit = glm::scale(modelNanosuit, glm::vec3(0.2f, 0.2f, 0.2f));
loading.setMat4("model", modelNanosuit);
firstmodel.drawMesh(&loading);

modelNanosuit = glm::mat4(1.0f);
modelNanosuit = glm::translate(modelNanosuit, glm::vec3(-10.0f, -1.0f, 0.0f));
modelNanosuit = glm::scale(modelNanosuit, glm::vec3(0.2f, 0.2f, 0.2f));
loading.setMat4("model", modelNanosuit);
cityModel.drawMesh(&loading);


dispShader.Win32useShader();
dispShader.setMat4("projection", projection);
dispShader.setMat4("view", view);

glm::mat4 model = glm::mat4(1.0f);
model = glm::rotate(model, glm::radians((float)glfwGetTime() * -10.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
dispShader.setMat4("model", model);
dispShader.setVec3("viewPos", cam.getCamPos());
dispShader.setVec3("lightPos", lightPos);
dispShader.setFloat("heightScale", 0.1f);

bindTexture2D(0, diffuseMapForNormals);
bindTexture2D(1, normalMap);
bindTexture2D(2, dispMap);

renderQuad();


model = glm::mat4(1.0f);
model = glm::translate(model, lightPos);
model = glm::scale(model, glm::vec3(0.1f));
dispShader.setMat4("model", model);
renderQuad();

floorShader.Win32useShader();

floorShader.setMat4("projection", projection);
floorShader.setMat4("view", view);

floorShader.setVec3("viewPos", cam.camPos);
floorShader.setVec3("lightPos", lightposfloor);

renderQuad();
*/
//sky.renderSkybox(&cubeMap, &cam, view, projection, cubemaptexture);


//parallax mapping example




//lightPos.x = sin(glfwGetTime()) * 3.0f;
           //lightPos.z = cos(glfwGetTime()) * 2.0f;
           //lightPos.y = 5.0 + cos(glfwGetTime()) * 1.0f;


           /*glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
           glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

           projection = glm::perspective(glm::radians(45.0f), float(WIDTH) / float(HEIGHT), 0.1f, 100.0f);
           view = glm::mat4(XEngine::getViewMatrix(&cam));
           model = glm::mat4(1.0f);

           Win32UseShader(&bloomShader);
           setMat4(&bloomShader, "projection", projection);
           setMat4(&bloomShader, "view", view);

           glActiveTexture(GL_TEXTURE0);
           glBindTexture(GL_TEXTURE_2D, diffuseMapTexture);

           for (uint16 i = 0; i < lightPositions.size(); ++i)
           {
               setVec3(&bloomShader, "light[" + std::to_string(i) + "].Pos", lightPositions[i]);
               setVec3(&bloomShader, "light[" + std::to_string(i) + "].Color", lightColors[i]);
           }

           setVec3(&bloomShader, "viewPos", cam.camPos);

           model = glm::mat4(1.0f);
           model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0));
           model = glm::scale(model, glm::vec3(12.5f, 0.5f, 12.5f));
           setMat4(&bloomShader, "model", model);
           setMat4(&bloomShader, "model", model);
           renderCube();

           glBindTexture(GL_TEXTURE_2D, specularMap);

           model = glm::mat4(1.0f);
           model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
           model = glm::scale(model, glm::vec3(0.5f));
           setMat4(&bloomShader, "model", model);
           renderCube();

           model = glm::mat4(1.0f);
           model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
           model = glm::scale(model, glm::vec3(0.5f));
           setMat4(&bloomShader, "model", model);
           renderCube();

           model = glm::mat4(1.0f);
           model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 2.0));
           model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
           setMat4(&bloomShader, "model", model);
           renderCube();

           model = glm::mat4(1.0f);
           model = glm::translate(model, glm::vec3(0.0f, 2.7f, 4.0));
           model = glm::rotate(model, glm::radians(23.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
           model = glm::scale(model, glm::vec3(1.25));
           setMat4(&bloomShader, "model", model);
           renderCube();

           model = glm::mat4(1.0f);
           model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -3.0));
           model = glm::rotate(model, glm::radians(124.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
           setMat4(&bloomShader, "model", model);
           renderCube();

           model = glm::mat4(1.0f);
           model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0));
           model = glm::scale(model, glm::vec3(0.5f));
           setMat4(&bloomShader, "model", model);
           renderCube();

           // finally show all the light sources as bright cubes
           Win32UseShader(&lightShader);
           setMat4(&lightShader, "projection", projection);
           setMat4(&lightShader, "view", view);

           for (unsigned int i = 0; i < lightPositions.size(); i++)
           {
               model = glm::mat4(1.0f);
               model = glm::translate(model, glm::vec3(lightPositions[i]));
               model = glm::scale(model, glm::vec3(0.25f));
               setMat4(&lightShader, "model", model);
               setVec3(&lightShader, "lightColor", lightColors[i]);
               renderCube();
           }
           glBindFramebuffer(GL_FRAMEBUFFER, 0);

           bool hor = true, first_i = true;
           unsigned int amount = 10;

           Win32UseShader(&blurShader);
           for (unsigned int i = 0; i < amount; i++)
           {
               glBindFramebuffer(GL_FRAMEBUFFER, pingphongFBO[hor]);
               setInt(&blurShader, "hor", hor);
               glBindTexture(GL_TEXTURE_2D, first_i ? colorBuffers[1] : pingcolorBuffer[!hor]);  // bind texture of other framebuffer (or scene if first iteration)
               renderQuad();
               hor = !hor;
               if (first_i)
                   first_i = false;
           }
           glBindFramebuffer(GL_FRAMEBUFFER, 0);

           glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
           Win32UseShader(&mixedShader);
           glActiveTexture(GL_TEXTURE0);
           glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
           glActiveTexture(GL_TEXTURE1);
           glBindTexture(GL_TEXTURE_2D, pingcolorBuffer[!hor]);
           setInt(&mixedShader, "bloom", bloom);
           setFloat(&mixedShader, "exposure", exposure);
           renderQuad();*/

           /* //plane
            Win32UseShader(&floorShader);

            setMat4(&floorShader, "projection", projection);
            setMat4(&floorShader, "view", view);

            setVec3(&floorShader, "viewPos", cam.camPos);
            setVec3(&floorShader, "lightPos", lightposfloor);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, floorTexture);

            glBindVertexArray(planeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);*/




            //skybox
            /*glDepthFunc(GL_LEQUAL);
            Win32UseShader(&cubeMap);
            view = glm::mat4(glm::mat3(XEngine::getViewMatrix(&cam)));
            setMat4(&cubeMap, "projection", projection);
            setMat4(&cubeMap, "view", view);

            glBindVertexArray(skyboxVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemaptexture);

            glDrawArrays(GL_TRIANGLES, 0, 36);

            glBindVertexArray(0);
            glDepthFunc(GL_LESS);*/

            /*
            lights
            Win32UseShader(&lightShader);
            setMat4(&lightShader, "projection", projection);
            setMat4(&lightShader, "view", view);

            glBindVertexArray(lightVAO);
            for (unsigned int i = 0; i < 4; i++)
            {
                model = glm::mat4(1.0f);
                model = glm::translate(model, pointLightPositions[i]);
                model = glm::scale(model, glm::vec3(0.2f));
                setMat4(&lightShader, "model", model);
                glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 1);
            }*/



            //cubes
            /*glm::mat4 modelshadow = glm::mat4(1.0f);
            setMat4(&shadowShader, "model", modelshadow);
            glBindVertexArray(planeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            modelshadow = glm::mat4(1.0f);
            modelshadow = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
            modelshadow = glm::scale(model, glm::vec3(0.5f));
            setMat4(&shadowShader, "model", model);
            renderCube();*/


            /*Win32UseShader(&basicShader);

            setVec3(&basicShader, "viewPos", cam.camPos);
            setFloat(&basicShader, "material.shininess", 64.0f);

            setVec3(&basicShader, "dirLight.direction", -0.2f, -1.0f, -0.3f);
            setVec3(&basicShader, "dirLight.ambient", 0.05f, 0.05f, 0.05f);
            setVec3(&basicShader, "dirLight.diffuse", 1.0f, 0.1f, 0.1f);
            setVec3(&basicShader, "dirLight.specular", 0.5f, 0.5f, 0.5f);

            for (int i = 0; i < 4; ++i)
            {
                setVec3(&basicShader, "pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);
                setVec3(&basicShader, "pointLights[" + std::to_string(i) + "].ambient", 0.05f, 0.05f, 0.05f);
                setVec3(&basicShader, "pointLights[" + std::to_string(i) + "].diffuse", 1.0f, 1.0f, 1.0f);
                setVec3(&basicShader, "pointLights[" + std::to_string(i) + "].specular", 1.0f, 1.0f, 1.0f);
                setFloat(&basicShader, "pointLights[" + std::to_string(i) + "].c", 1.0f);
                setFloat(&basicShader, "pointLights[" + std::to_string(i) + "].linear", 0.09f);
                setFloat(&basicShader, "pointLights[" + std::to_string(i) + "].quadratic", 0.032f);
            }

            setVec3(&basicShader, "spotLight.position", cam.camPos);
            setVec3(&basicShader, "spotLight.direction", cam.camTarget);
            setVec3(&basicShader, "spotLight.ambient", 0.0f, 0.0f, 1.0f);
            setVec3(&basicShader, "spotLight.diffuse", 0.0f, 0.0f, 1.0f);
            setVec3(&basicShader, "spotLight.specular", 1.0f, 1.0f, 1.0f);
            setFloat(&basicShader, "spotLight.constant", 1.0f);
            setFloat(&basicShader, "spotLight.linear", 0.09f);
            setFloat(&basicShader, "spotLight.quadratic", 0.032f);
            setFloat(&basicShader, "spotLight.cutOff", glm::cos(glm::radians(12.5f)));
            setFloat(&basicShader, "spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));


            setMat4(&basicShader, "projection", projection);
            setMat4(&basicShader, "view", view);

            glm::mat4 modeln = glm::mat4(1.0f);
            setMat4(&basicShader, "model", modeln);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, diffuseMap);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, specularMap);

            glBindVertexArray(VAO);
            for (unsigned int i = 0; i < 10; i++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, cubePositions[i]);
                float angle = 20.0f * i;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                setMat4(&basicShader, "model", model);
                glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 1);
            }
            */

#endif

#if 0
Win32UseShader(&lightMapShader);
setMat4(&lightMapShader, "lightmatrix", lightspaceMatrix);

glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

glClear(GL_DEPTH_BUFFER_BIT);
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, floorTexture);

//render

renderScene(&lightMapShader, planeVAO);

glBindFramebuffer(GL_FRAMEBUFFER, 0);

glViewport(0, 0, WIDTH, HEIGHT);
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

glViewport(0, 0, WIDTH, HEIGHT);
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

Win32UseShader(&shadowShader);
projection = glm::perspective(glm::radians(45.0f), (real32)WIDTH / (real32)HEIGHT, 0.1f, 100.0f);
view = XEngine::getViewMatrix(&cam);

setMat4(&shadowShader, "projection", projection);
setMat4(&shadowShader, "view", view);

setVec3(&shadowShader, "viewPos", cam.camPos);
setVec3(&shadowShader, "lightPos", lightPos);
setMat4(&shadowShader, "lightSpaceMatrix", lightspaceMatrix);

glActiveTexture(GL_TEXTURE0);
glBindTexture(TEXTURE2D, floorTexture);
glActiveTexture(GL_TEXTURE1);
glBindTexture(TEXTURE2D, depthMap);

renderScene(&shadowShader, planeVAO);

Win32UseShader(&depthMapQuad);
setFloat(&depthMapQuad, "near_plane", nearp);
setFloat(&depthMapQuad, "far_plane", farp);
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, depthMap);


void renderScene(Shader *shader, unsigned int &vao)
{
    glm::mat4 model = glm::mat4(1.0f);
    shader->setMat4("model", model);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // cubes
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader->setMat4("model", model);
    renderCube();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader->setMat4("model", model);
    renderCube();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
    model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.25));
    shader->setMat4("model", model);
    renderCube();
}

#endif


