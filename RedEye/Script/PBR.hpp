#pragma once

#include "Engine/Engine.hpp"
#include "Component/Component.hpp"
#include "RedLog/Debug.hpp"
#include "RedGL/RedGL.hpp"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <glad/glad.h>

#include <File/IMAGE/stb_image.h>

class PBR : public RedEyeBehaviour{
public:
    GLRenderTarget * target;

    Screen * screen = Screen::getInstance();

    GLProgram * equirectangularToCubemap;
    GLProgram * irradianceShader;
    GLProgram * background;
    GLProgram * prefilterShader;
    GLProgram * brdfShader;

    Camera * mainCamera;


    unsigned int envCubemap;
    unsigned int irradianceMap;
    unsigned int brdfLUTTexture;
    unsigned int prefilterMap;


    GLProgram * pbr;


    Model * teaModel;

    Light * light0;
    Light * light1;
    Light * light2;
    Light * light3;

    GLTexture * albedoMap;
    GLTexture * metallicMap;
    GLTexture * roughnessMap;
    GLTexture * normalMap;
    GLTexture * aoMap;





    void Start(){

        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        Screen * screen = Screen::getInstance();
        mainCamera = new Camera(30.0f,screen->getWidth(),screen->getHeight(),0.1f,1000.0f);


        pbr = new GLProgram(PBR_VERTEX,PBR_FRAGMENT);
        equirectangularToCubemap = new GLProgram(cubemap_vertex_shader,cubemap_fragment_shader);
        background = new GLProgram(background_vertex_shader,background_fragment_shader);
        irradianceShader = new GLProgram(irradiance_vertex_shader,irradiance_fragment_shader);
        prefilterShader = new GLProgram(prefilter_vertex_shader,prefilter_fragment_shader);
        brdfShader = new GLProgram(brdf_vertex_shader,brdf_fragment_shader);

        //teaModel = new Model("/Users/redknot/Red3DEngine/3dModel/Cerberus_by_Andrew_Maximov/Cerberus_LP");
        teaModel = new Model("C://RedEyeEngine/res/3dcoat/M");

		albedoMap = new GLTexture();
		albedoMap->LoadImage("C://RedEyeEngine/res/3dcoat/color.jpg");
		metallicMap = new GLTexture();
		metallicMap->LoadImage("C://RedEyeEngine/res/3dcoat/metalness.jpg");
		roughnessMap = new GLTexture();
		roughnessMap->LoadImage("C://RedEyeEngine/res/3dcoat/gloss.jpg");
		normalMap = new GLTexture();
		normalMap->LoadImage("C://RedEyeEngine/res/3dcoat/nmap.jpg");
		aoMap = new GLTexture();
		aoMap->LoadImage("C://RedEyeEngine/res/3dcoat/ao.jpg");

		/*
        albedoMap = new GLTexture();
        albedoMap->LoadImage("c://Red3DEngine/3dModel/coat/T/export3dcoat_lambert3SG_color.jpeg");
        metallicMap = new GLTexture();
        metallicMap->LoadImage("c://Red3DEngine/3dModel/coat/T/export3dcoat_lambert3SG_metalness.jpeg");
        roughnessMap = new GLTexture();
        roughnessMap->LoadImage("c://Red3DEngine/3dModel/coat/T/export3dcoat_lambert3SG_gloss.jpeg");
        normalMap = new GLTexture();
        normalMap->LoadImage("c://Red3DEngine/3dModel/coat/T/export3dcoat_lambert3SG_nmap.jpg");
        aoMap = new GLTexture();
        aoMap->LoadImage("c://Red3DEngine/3dModel/coat/T/materialball_ao.jpg");
		*/

        /*
        albedoMap = new GLTexture();
        albedoMap->LoadImage("/Users/redknot/Red3DEngine/3dModel/Tea/T3/tea_DefaultMaterial_BaseColor.png");
        metallicMap = new GLTexture();
        metallicMap->LoadImage("/Users/redknot/Red3DEngine/3dModel/Tea/T3/tea_DefaultMaterial_Metallic.png");
        roughnessMap = new GLTexture();
        roughnessMap->LoadImage("/Users/redknot/Red3DEngine/3dModel/Tea/T3/tea_DefaultMaterial_Roughness.png");
        normalMap = new GLTexture();
        normalMap->LoadImage("/Users/redknot/Red3DEngine/3dModel/Tea/T3/tea_DefaultMaterial_Normal.png");
        aoMap = new GLTexture();
        aoMap->LoadImage("/Users/redknot/Red3DEngine/3dModel/Tea/T3/tea_DefaultMaterial_Height.png");
        */

        /*
        albedoMap = new GLTexture();
        albedoMap->LoadImage("/Users/redknot/Red3DEngine/3dModel/Cerberus_by_Andrew_Maximov/T/Cerberus_A.png");
        metallicMap = new GLTexture();
        metallicMap->LoadImage("/Users/redknot/Red3DEngine/3dModel/Cerberus_by_Andrew_Maximov/T/Cerberus_M.png");
        roughnessMap = new GLTexture();
        roughnessMap->LoadImage("/Users/redknot/Red3DEngine/3dModel/Cerberus_by_Andrew_Maximov/T/Cerberus_R.png");
        normalMap = new GLTexture();
        normalMap->LoadImage("/Users/redknot/Red3DEngine/3dModel/Cerberus_by_Andrew_Maximov/T/Cerberus_N.png");
        aoMap = new GLTexture();
        aoMap->LoadImage("/Users/redknot/Red3DEngine/3dModel/Cerberus_by_Andrew_Maximov/T/Cerberus_AO.png");
        */


        light0 = new Light();
        light0->setColor(300.0f,300.0f,300.0f);
        light0->setPosition(-10.0f,  10.0f, 10.0f);

        light1 = new Light();
        light1->setColor(300.0f,300.0f,300.0f);
        light1->setPosition(10.0f,  10.0f, 10.0f);

        light2 = new Light();
        light2->setColor(300.0f,300.0f,300.0f);
        light2->setPosition(-10.0f, -10.0f, 10.0);

        light3 = new Light();
        light3->setColor(300.0f,300.0f,300.0f);
        light3->setPosition(10.0f, -10.0f, 10.0f);

        target = new GLRenderTarget(screen->getWidth(),screen->getHeight(),RED_TARGET_SCREEN);

		GLTexture * HrdTexture = new GLTexture();
		HrdTexture->LoadHdrImage("c://Red3DEngine/Texture/hdr/newport_loft.hdr");

		GLuint hdrTexture = HrdTexture->TextureId;

        // pbr: setup framebuffer
            // ----------------------
        unsigned int captureFBO;
            unsigned int captureRBO;
        glGenFramebuffers(1, &captureFBO);
            glGenRenderbuffers(1, &captureRBO);

            glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
            glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 2048, 2048);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);


        // pbr: setup cubemap to render to and attach to framebuffer
        // ---------------------------------------------------------

        glGenTextures(1, &envCubemap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 2048, 2048, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


        // pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
        // ----------------------------------------------------------------------------------------------
        glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        glm::mat4 captureViews[] =
        {
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
        };

        equirectangularToCubemap->UseProgram();
        equirectangularToCubemap->put1i("equirectangularMap",0);
        equirectangularToCubemap->putMatrix4fv("projection", glm::value_ptr(captureProjection));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, hdrTexture);

        glViewport(0, 0, 2048, 2048);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        for (unsigned int i = 0; i < 6; ++i)
        {
            equirectangularToCubemap->putMatrix4fv("view", glm::value_ptr(captureViews[i]));
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


            renderCube();
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        // pbr: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
        // --------------------------------------------------------------------------------

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

        irradianceShader->UseProgram();
        irradianceShader->put1i("environmentMap", 0);
        irradianceShader->putMatrix4fv("projection", glm::value_ptr(captureProjection));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

        glViewport(0, 0, 32, 32);
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

        for (unsigned int i = 0; i < 6; ++i)
        {
            irradianceShader->putMatrix4fv("view", glm::value_ptr(captureViews[i]));
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            renderCube();
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        // pbr: create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
        // --------------------------------------------------------------------------------

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
        prefilterShader->UseProgram();
        prefilterShader->put1i("environmentMap",0);
        prefilterShader->putMatrix4fv("projection", glm::value_ptr(captureProjection));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        unsigned int maxMipLevels = 5;
        for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
        {
                // reisze framebuffer according to mip-level size.
            unsigned int mipWidth  = 128 * std::pow(0.5, mip);
            unsigned int mipHeight = 128 * std::pow(0.5, mip);
            glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
            glViewport(0, 0, mipWidth, mipHeight);

            float roughness = (float)mip / (float)(maxMipLevels - 1);
            prefilterShader->put1f("roughness",roughness);

            for (unsigned int i = 0; i < 6; ++i)
            {
                prefilterShader->putMatrix4fv("view", glm::value_ptr(captureViews[i]));
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                renderCube();
            }
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // pbr: generate a 2D LUT from the BRDF equations used.
        // ----------------------------------------------------

        glGenTextures(1, &brdfLUTTexture);

        // pre-allocate enough memory for the LUT texture.
        glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
        // be sure to set wrapping mode to GL_CLAMP_TO_EDGE
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

        glViewport(0, 0, 512, 512);
        brdfShader->UseProgram();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderQuad();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    float w = 0.0f;

    void Update(){
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        Mouse();
        target->setWidthAndHeight(screen->getWidth(),screen->getHeight());
        target->useFrameBuffer();
        mainCamera->setCameraWidthHeight(screen->getWidth(),screen->getHeight());


        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glEnable(GL_DEPTH_TEST);

        background->UseProgram();
        background->putMatrix4fv("view",glm::value_ptr(mainCamera->getView()));
        background->putMatrix4fv("projection",glm::value_ptr(mainCamera->getProjection()));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
        renderCube();



        glm::mat4 model;
        model = glm::scale(model,glm::vec3(0.3f));
        //model = glm::rotate(model,glm::radians(90.0f),glm::vec3(-1.0f,0.0f,0.0f));
        model = glm::rotate(model,glm::radians(w),glm::vec3(1.0f,0.0f,0.0f));
        w = w + 0.5f;
        pbr->putMatrix4fv("model",glm::value_ptr(model));
        pbr->putMatrix4fv("projection",glm::value_ptr(mainCamera->getProjection()));
        pbr->putMatrix4fv("view",glm::value_ptr(mainCamera->getView()));
        pbr->put3f("camPos",mainCamera->cameraPos[0],mainCamera->cameraPos[1],mainCamera->cameraPos[2]);


            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, this->albedoMap->TextureId);
            pbr->put1i("albedoMap",0);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, this->metallicMap->TextureId);
            pbr->put1i("metallicMap",1);

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, this->roughnessMap->TextureId);
            pbr->put1i("roughnessMap",2);

            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, this->normalMap->TextureId);
            pbr->put1i("normalMap",3);

            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, this->aoMap->TextureId);
            pbr->put1i("aoMap",4);

            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
            pbr->put1i("irradianceMap",5);

            glActiveTexture(GL_TEXTURE6);
            glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
            pbr->put1i("prefilterMap",6);

            glActiveTexture(GL_TEXTURE7);
            glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
            pbr->put1i("brdfLUT",7);


        light0->UseLight(pbr,0);
        light1->UseLight(pbr,1);
        light2->UseLight(pbr,2);
        light3->UseLight(pbr,3);

        pbr->UseProgram();
        teaModel->DrawAllVAO();
    }

    void End(){
        delete target;
    }

    unsigned int cubeVAO = 0;
    unsigned int cubeVBO = 0;

    void renderCube()
    {
        // initialize (if necessary)
        if (cubeVAO == 0)
        {
            float vertices[] = {
                // back face
				1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
                1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
				-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
				-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
                -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
				1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
                // front face
				1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
                1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
				-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left    
				-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
                -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
				1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
                // left face
				-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
                -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
				-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
				-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
                -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
				-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
                // right face
				1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right
                1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
				1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
				1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left
                1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
				1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
                
                // bottom face
				1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
                1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
				-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
				-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
                -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
				1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
                
                // top face
				1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right
                1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
				-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
				-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,  // bottom-left
                -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
				1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
                
            };

            glGenVertexArrays(1, &cubeVAO);
            glGenBuffers(1, &cubeVBO);
            // fill buffer
            glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            // link vertex attributes
            glBindVertexArray(cubeVAO);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
        // render Cube
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }


    unsigned int quadVAO = 0;
    unsigned int quadVBO;
    void renderQuad()
    {
        if (quadVAO == 0)
        {
            float quadVertices[] = {
                // positions        // texture Coords
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            };
            // setup plane VAO
            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &quadVBO);
            glBindVertexArray(quadVAO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        }
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }


private:
    bool firstMouse = true;
    float lastX = 0.0f;
    float lastY = 0.0f;
    float yaw;
    float pitch;
    void Mouse()
    {
        Input * input = Input::getInstance();

        float xpos = input->getMousePoint().x;
        float ypos = input->getMousePoint().y;

        if(firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
            return;
        }

        GLfloat xoffset = xpos - lastX;
        GLfloat yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        GLfloat sensitivity = 0.5f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw   += xoffset;
        pitch += yoffset;

        if(pitch > 89.0f)
            pitch = 89.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(front);

        this->mainCamera->setCameraFront(front.x,front.y,front.z);
    }


};
