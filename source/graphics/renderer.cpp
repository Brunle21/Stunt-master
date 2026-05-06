#include "renderer.h"
#include "camera.h"
#include "shader_manager.h"
#include "../utils/logger.h"

Renderer::Renderer()
    : mRoot(nullptr), mRenderWindow(nullptr), mSceneManager(nullptr) {
    Logger::info("Renderer", "Renderer constructor called");
}

Renderer::~Renderer() {
    shutdown();
}

bool Renderer::initialize() {
    Logger::info("Renderer", "Initializing Renderer...");

    if (!initializeOGRE()) {
        Logger::error("Renderer", "Failed to initialize OGRE");
        return false;
    }

    if (!createWindow()) {
        Logger::error("Renderer", "Failed to create render window");
        return false;
    }

    if (!setupScene()) {
        Logger::error("Renderer", "Failed to setup scene");
        return false;
    }

    // Initialize camera
    mCamera = std::make_unique<Camera>(mSceneManager);
    if (!mCamera->initialize()) {
        Logger::error("Renderer", "Failed to initialize camera");
        return false;
    }

    // Initialize shader manager
    mShaderManager = std::make_unique<ShaderManager>();
    if (!mShaderManager->initialize()) {
        Logger::error("Renderer", "Failed to initialize shader manager");
        return false;
    }

    Logger::info("Renderer", "Renderer initialized successfully");
    return true;
}

bool Renderer::initializeOGRE() {
    Logger::info("Renderer", "Initializing OGRE3D...");

    try {
        mRoot = new Ogre::Root();

        // This is a simplified initialization
        // In a real implementation, you would load plugins and configure rendering systems
        Logger::info("Renderer", "OGRE3D initialized successfully");
        return true;
    } catch (const std::exception& e) {
        Logger::error("Renderer", std::string("OGRE initialization failed: ") + e.what());
        return false;
    }
}

bool Renderer::createWindow() {
    Logger::info("Renderer", "Creating render window...");

    try {
        // Window parameters
        Ogre::NameValuePairList params;
        params["FSAA"] = "4";
        params["vsync"] = "true";

        mRenderWindow = mRoot->createRenderWindow(
            "Stunt Master",
            1920, 1080,
            false,
            &params
        );

        if (!mRenderWindow) {
            Logger::error("Renderer", "Failed to create render window");
            return false;
        }

        Logger::info("Renderer", "Render window created successfully");
        return true;
    } catch (const std::exception& e) {
        Logger::error("Renderer", std::string("Window creation failed: ") + e.what());
        return false;
    }
}

bool Renderer::setupScene() {
    Logger::info("Renderer", "Setting up scene...");

    try {
        mSceneManager = mRoot->createSceneManager(Ogre::ST_GENERIC);

        if (!mSceneManager) {
            Logger::error("Renderer", "Failed to create scene manager");
            return false;
        }

        // Set ambient light
        mSceneManager->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));

        // Create a basic light
        Ogre::Light* light = mSceneManager->createLight("MainLight");
        light->setPosition(20, 80, 50);

        Logger::info("Renderer", "Scene setup complete");
        return true;
    } catch (const std::exception& e) {
        Logger::error("Renderer", std::string("Scene setup failed: ") + e.what());
        return false;
    }
}

void Renderer::beginFrame() {
    if (mRoot) {
        mRoot->_frameStarted();
    }
}

void Renderer::render() {
    if (mRoot && mRenderWindow) {
        mRoot->renderOneFrame();
    }
}

void Renderer::endFrame() {
    if (mRoot) {
        mRoot->_frameEnded();
    }
}

void Renderer::shutdown() {
    Logger::info("Renderer", "Shutting down Renderer...");

    mShaderManager.reset();
    mCamera.reset();

    if (mRoot) {
        delete mRoot;
        mRoot = nullptr;
    }

    Logger::info("Renderer", "Renderer shutdown complete");
}
