/*! \file SeedInit.cpp
	\author	Danny Angelo Carminati Grein
	\brief Seed Initialization / Shutdown
*/


#include "SeedInit.h"
#include "Config.h"
#include "ResourceManager.h"
#include "ResourceLoader.h"
#include "Image.h"
#include "MemoryManager.h"
#include "Timer.h"
#include "Package.h"
#include "PackageManager.h"
#include "FileSystem.h"
#include "System.h"
#include "Music.h"
#include "Sound.h"
#include "SoundSystem.h"
#include "Screen.h"
#include "Log.h"
#include "SpriteObject.h"
#include "Font.h"
#include "Input.h"
#include "gui/GuiManager.h"
#include "gui/Button.h"
#include "CollisionMask.h"
#include "Dictionary.h"
#include "StringCache.h"
#include "ParticleEmitterObject.h"
#include "ParticleManager.h"
#include "StringPool.h"
#include "Updater.h"


namespace Seed {


namespace Private {
	IGameApp	*pApplication 	= NULL;
	IRenderer	*pRenderer 		= NULL;
	BOOL		bInitialized 	= FALSE;
	int			iArgc			= 0;
	char		**pcArgv		= NULL;
	f32			fCurrentTime	= 0.0f;
}

ResourceManager glResourceManager("global");

#if SEED_USE_STRING_POOL == 1
StringPoolManager<u16> glStringPool;
#endif // SEED_USE_STRING_POOL

INLINE void SetGameApp(IGameApp *app, int argc, char **argv)
{
	Private::iArgc = argc;
	Private::pcArgv = argv;
	Private::pApplication = app;
	Private::pApplication->Setup(argc, argv);
}

INLINE void SetRenderer(IRenderer *renderer)
{
	Private::pRenderer = renderer;
}

INLINE void WriteOut(const char *msg)
{
	if (Private::pApplication)
		Private::pApplication->WriteOut(msg);
}

INLINE void WriteErr(const char *msg)
{
	if (Private::pApplication)
		Private::pApplication->WriteErr(msg);
}

void Initialize()
{
	if (!Private::pApplication)
	{
		fprintf(stderr, "ERROR: You should set a GameApp by calling SetGameApp(IGameApp *app, int argc, char **argv)!\n");
		HALT;
	}

	Info(SEED_MESSAGE);
	Info(SEED_TAG "Initializing...");

	pSystem->Initialize();
	pMemoryManager->Initialize();
	pTimer->Initialize();
	pPackageManager->Initialize();
	pFileSystem->Initialize();
	pScreen->Initialize();
	pSoundSystem->Initialize();
	pGuiManager->Initialize();
	pResourceLoader->Initialize();
	pInput->Initialize();
	pDictionary->Initialize();
	pStringCache->Initialize();
	pParticleManager->Initialize();

	pUpdater->Add(Private::pApplication);
	pUpdater->Add(pInput);
	pUpdater->Add(pGuiManager);
	pUpdater->Add(pSoundSystem);
	pUpdater->Add(pSystem);
	pUpdater->Add(pResourceLoader);
	pUpdater->Add(pParticleManager);

	ResourceManager::Register(Seed::ObjectImage,			ImageResourceLoader);
	ResourceManager::Register(Seed::ObjectSprite,			SpriteResourceLoader);
	ResourceManager::Register(Seed::ObjectFont,				FontResourceLoader);
	ResourceManager::Register(Seed::ObjectPackage,			PackageResourceLoader);
	ResourceManager::Register(Seed::ObjectSound,			SoundResourceLoader);
	ResourceManager::Register(Seed::ObjectMusic,			MusicResourceLoader);
	ResourceManager::Register(Seed::ObjectGuiButton,		ButtonResourceLoader);
	ResourceManager::Register(Seed::ObjectCollisionMask,	CollisionMaskResourceLoader);
	ResourceManager::Register(Seed::ObjectParticleEmitterObject, ParticleEmitterObjectResourceLoader);

	//pSystem->InitializeHome(); FIXME home can only be initialized after the ROM package with the
	//default error font was called and this is done at app->Initialize since the memory manager is not
	//ready during Setup
	Private::bInitialized = TRUE;
	Private::pApplication->Initialize();
}

void Update()
{
	if (!Private::bInitialized)
		return;

	f32 newTime				= pTimer->GetMilliseconds() / 1000.0f;
	f32 dt					= newTime - Private::fCurrentTime;
	Private::fCurrentTime	= newTime;

	pUpdater->Run(dt, (1000.0f / 60.0f / 1000.0f)); //60

	//pInput->Update(dt);
	//Private::pApplication->Update(dt);
	/*pGuiManager->Update(dt);
	pSoundSystem->Update(dt);
	pSystem->Update(dt);
	pResourceLoader->Update(dt);
	pParticleManager->Update(dt);*/
	//Private::pRenderer->Update();

	Seed::Render(dt);
}

void Render(f32 delta)
{
#if !defined(_QT_)
	Private::pRenderer->Render(delta);
#endif
	pScreen->Update(delta);
}

void Shutdown()
{
	if (!Private::bInitialized)
		return;

	Private::pApplication->Shutdown();

	// Reset util classes
	pDictionary->Reset();
	glResourceManager.Reset();

	pParticleManager->Shutdown();
	pStringCache->Shutdown();
	pDictionary->Shutdown();
	pInput->Shutdown();
	pResourceLoader->Shutdown();
	pGuiManager->Shutdown();
	pSoundSystem->Shutdown();
	pScreen->Shutdown();
	pFileSystem->Shutdown();
	pPackageManager->Shutdown();
	pTimer->Shutdown();
	pMemoryManager->Shutdown();
	Info(SEED_TAG "Terminating...");
	pSystem->Shutdown();

	Private::bInitialized = FALSE;
}


} // namespace