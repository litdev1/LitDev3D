// LitDev3DCPP.h

#pragma once

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Irrlicht.lib")
#endif

#include <irrlicht.h>
#include <windows.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#define ACTION_NONE 0
#define ACTION_ADDSKYBOX 1
#define ACTION_ADDSKYDOME 2
#define ACTION_ADDCUBE 3
#define ACTION_ADDSPHERE 4
#define ACTION_REMOVE 5
#define ACTION_ADDANIMATEDMESH 6
#define ACTION_ADDTERRAIN 7
#define ACTION_ADDZIP 8
#define ACTION_ADDQUAKEMESH 9
#define ACTION_SETTEXTURE 10
#define ACTION_SETPOSITION 11
#define ACTION_SETSCALE 12
#define ACTION_SETROTATION 13
#define ACTION_SETVISIBLE 14
#define ACTION_ANIMATETO 15
#define ACTION_ANIMATEROTATION 16
#define ACTION_REMOVEANIMATORS 17
#define ACTION_SETGRAVITY 18
#define ACTION_ANIMATERELATIVE 19
#define ACTION_ANIMATEONTERRAINTO 20
#define ACTION_ANIMATEONTERRAINRELATIVE 21
#define ACTION_SETGLOW 22
#define ACTION_ADDLIGHT 23
#define ACTION_AMBIENTLIGHT 24
#define ACTION_ADDFOG 25
#define ACTION_SETSHININESS 26
#define ACTION_SETCAMERAPROPERTIES 27
#define ACTION_ANIMATECIRCLE 28
#define ACTION_ADDBILLBOARD 29

namespace LitDev3D
{
	class Action
	{
	public:
		char node[_MAX_PATH];
		char mesh[_MAX_PATH];
		int action;
		char up[_MAX_PATH], dn[_MAX_PATH], lf[_MAX_PATH], rt[_MAX_PATH], ft[_MAX_PATH], bk[_MAX_PATH];
		char dome[_MAX_PATH];
		int startFrame, endFrame, fps;
		char height[_MAX_PATH], terrain[_MAX_PATH], detail[_MAX_PATH];
		float gravity, time, radius, density, shininess, speed, farfield;
		float X, Y, Z;
		int R, G, B;
		bool visible;
		char texture[_MAX_PATH];
		char fireball[_MAX_PATH], particlewhite[_MAX_PATH];

		Action()
		{
			action = ACTION_NONE;
		}

		Action(char* _node, int _action)
		{
			strcpy(node, _node);
			action = _action;
		}

		~Action()
		{
		}
	};

	class EventReceiver : public IEventReceiver
	{
	private:
		bool KeyIsDown[KEY_KEY_CODES_COUNT];
		bool KeyWasDown[KEY_KEY_CODES_COUNT];
	public:
		bool LeftMouse, RightMouse;

		virtual bool OnEvent(const SEvent& event)
		{
			if (event.EventType == EET_KEY_INPUT_EVENT)
			{
				KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
				KeyWasDown[event.KeyInput.Key] |= event.KeyInput.PressedDown;
			}
			else if (event.EventType == EET_MOUSE_INPUT_EVENT)
			{
				LeftMouse = event.MouseInput.isLeftPressed();
				RightMouse = event.MouseInput.isRightPressed();
			}
			return false;
		}

		virtual bool IsKeyDown(EKEY_CODE keyCode) const
		{
			return KeyIsDown[keyCode];
		}

		bool WasKeyDown(EKEY_CODE keyCode)
		{
			bool keystate = KeyWasDown[keyCode];
			KeyWasDown[keyCode] = false;
			return keystate;
		}

		void resetAllKeyDown()
		{
			for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
			{
				KeyWasDown[i] = false;
			}
		}

		EventReceiver()
		{
			for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
			{
				KeyIsDown[i] = false;
				KeyWasDown[i] = false;
			}
			LeftMouse = false;
			RightMouse = false;
		}

		~EventReceiver()
		{
		}
	};

	class EngineCPP
	{
	private:
		char resourcePath[_MAX_PATH];
		typedef void (*MessageType)(int value, char* strValue1, char* strValue2);
		char strValue1[_MAX_PATH], strValue2[_MAX_PATH];
		char objectType[_MAX_PATH];
		MessageType sendMessage;
		IrrlichtDevice* device;
		EventReceiver receiver;
		IVideoDriver* driver;
		ISceneManager* smgr;
		SColor backColor;
		ICameraSceneNode* camera;
		ITriangleSelector* selector;
		float SizeX, SizeY;
		bool running;
		bool clickHandled;
		bool doingActions;
		bool consoleMessages;
		bool emf_lighting;
		bool emf_fog_enable;
		float pi;
		list<Action> Actions;
	public:
		EngineCPP()
		{
		}
		EngineCPP(void* Message)
		{
			sendMessage = (MessageType)(Message);
			strcpy(strValue1,"");
			strcpy(strValue2,"");
			strcpy(objectType,"");
			clickHandled = true;
			running = false;
			doingActions = false;
			consoleMessages = false;
			emf_lighting = true;
			emf_fog_enable = true;
			device = 0;
			driver = 0;
			smgr = 0;
			camera = 0;
			selector = 0;
			pi = 2 * acos(0.0);
		}
		~EngineCPP()
		{
		}

		bool wasKeyDown(int key)
		{
			return receiver.WasKeyDown((EKEY_CODE) key);
		}

		void resetAllKeyDown()
		{
			receiver.resetAllKeyDown();
		}

		void comment(char* str1, char* str2)
		{
			if (!consoleMessages) return;
			strcpy(strValue1,str1);
			strcpy(strValue2,str2);
			sendMessage(99, strValue1, strValue2);
		}

		void addAction(Action action)
		{
			while (doingActions) Sleep(1);
			doingActions = true;
			Actions.push_back(action);
			doingActions = false;
		}

		void doActions()
		{
			while (doingActions) Sleep(1);
			doingActions = true;
			list<Action>::ConstIterator act = Actions.begin(); 
			for (; act != Actions.end(); ++act)
			{
				Action action = *act;
				switch (action.action)
				{
				case ACTION_NONE:
					break;
				case ACTION_ADDSKYBOX:
					_addSkybox(action.up, action.dn, action.lf, action.rt, action.ft, action.bk);
					break;
				case ACTION_ADDSKYDOME:
					_addSkydome(action.dome);
					break;
				case ACTION_ADDCUBE:
					_addCube(action.node);
					break;
				case ACTION_ADDSPHERE:
					_addSphere(action.node);
					break;
				case ACTION_REMOVE:
					_remove(action.node);
					break;
				case ACTION_ADDANIMATEDMESH:
					_addAnimatedMesh(action.node, action.mesh, action.startFrame, action.endFrame, action.fps);
					break;
				case ACTION_ADDTERRAIN:
					_addTerrain(action.node, action.height, action.terrain, action.detail, action.gravity, action.X, action.Y);
					break;
				case ACTION_ADDZIP:
					_addZip(action.node);
					break;
				case ACTION_ADDQUAKEMESH:
					_addQuakeMesh(action.node, action.mesh, action.gravity);
					break;
				case ACTION_SETTEXTURE:
					_setTexture(action.node, action.texture);
					break;
				case ACTION_SETPOSITION:
					_setPosition(action.node, action.X, action.Y, action.Z);
					break;
				case ACTION_SETSCALE:
					_setScale(action.node, action.X, action.Y, action.Z);
					break;
				case ACTION_SETROTATION:
					_setRotation(action.node, action.X, action.Y, action.Z);
					break;
				case ACTION_SETVISIBLE:
					_setVisible(action.node, action.visible);
					break;
				case ACTION_ANIMATETO:
					_animateTo(action.node, action.X, action.Y, action.Z, action.time);
					break;
				case ACTION_ANIMATEROTATION:
					_animateRotation(action.node, action.X, action.Y, action.Z);
					break;
				case ACTION_REMOVEANIMATORS:
					_removeAnimators(action.node);
					break;
				case ACTION_SETGRAVITY:
					_setGravity(action.node, action.gravity);
					break;
				case ACTION_ANIMATERELATIVE:
					_animateRelative(action.node, action.X, action.Y, action.Z, action.time);
					break;
				case ACTION_ANIMATEONTERRAINTO:
					_animateOnTerrainTo(action.node, action.X, action.Z, action.time);
					break;
				case ACTION_ANIMATEONTERRAINRELATIVE:
					_animateOnTerrainRelative(action.node, action.X, action.Z, action.time);
					break;
				case ACTION_SETGLOW:
					_setGlow(action.node, action.R, action.G, action.B, action.radius);
					break;
				case ACTION_ADDLIGHT:
					_addLight(action.node, action.X, action.Y, action.Z, action.R, action.G, action.B, action.radius, action.fireball, action.particlewhite);
					break;
				case ACTION_AMBIENTLIGHT:
					_ambientLight(action.R, action.G, action.B);
					break;
				case ACTION_ADDFOG:
					_addFog(action.R, action.G, action.B, action.density);
					break;
				case ACTION_SETSHININESS:
					_setShininess(action.node, action.shininess);
					break;
				case ACTION_SETCAMERAPROPERTIES:
					_setCameraProperties(action.speed, action.farfield, action.gravity);
					break;
				case ACTION_ANIMATECIRCLE:
					_animateCircle(action.node, action.radius, action.speed);
					break;
				case ACTION_ADDBILLBOARD:
					_addBillBoard(action.node, action.X, action.Y);
					break;
				}
			}
			Actions.clear();
			doingActions = false;
		}

		void doCollisions()
		{
			list<ISceneNode*>::ConstIterator it = smgr->getRootSceneNode()->getChildren().begin(); 
			list<ISceneNode*>::ConstIterator end = smgr->getRootSceneNode()->getChildren().end();
			ISceneNode *node1, *node2;
			for (; it != end-1; ++it)
			{
				node1 = *it;
				if (strstr(node1->getName(),"Node"))
				{
					list<ISceneNode*>::ConstIterator jt = it+1;
					for (; jt != end; ++jt)
					{
						node2 = *jt;
						if (strstr(node2->getName(),"Node") && node1->getTransformedBoundingBox().intersectsWithBox(node2->getTransformedBoundingBox()))
						{
							strcpy(strValue1, node1->getName());
							strcpy(strValue2, node2->getName());
							sendMessage(1, strValue1, strValue2);
						}
					}
				}
			}
		}

		void doClicked()
		{
			if (clickHandled && (receiver.LeftMouse || receiver.RightMouse))
			{
				ISceneCollisionManager* collMan = smgr->getSceneCollisionManager();
				ISceneNode *node = collMan->getSceneNodeFromCameraBB(camera);
				if (node && strstr(node->getName(),"Node"))
				{
					strcpy(strValue1, node->getName());
					strcpy(strValue2, "");
					sendMessage(2, strValue1, strValue2);
					clickHandled = false;
				}
			}
			else if (!receiver.LeftMouse && !receiver.RightMouse)
			{
				clickHandled = true;
			}
		}

		void cleanAnimators(ISceneNode* node, ISceneNodeAnimator* anim)
		{
			if (node->getAnimators().getSize() > 0)
			{
				list<ISceneNodeAnimator*> animators;
				list<ISceneNodeAnimator*>::ConstIterator it;
				it = node->getAnimators().begin();
				for (; it != node->getAnimators().end(); ++it)
				{
					if ((*it)->getType() == anim->getType()) animators.push_back(*it);
				}
				it = animators.begin();
				for (; it != animators.end(); ++it)
				{
					node->removeAnimator(*it);
				}
			}
		}

		void setCamera(char* cameraName, float speed, float farfield, float gravity)
		{
			SKeyMap keyMap[8];
			keyMap[0].Action = EKA_MOVE_FORWARD;
			keyMap[0].KeyCode = KEY_UP;
			keyMap[1].Action = EKA_MOVE_FORWARD;
			keyMap[1].KeyCode = KEY_KEY_W;

			keyMap[2].Action = EKA_MOVE_BACKWARD;
			keyMap[2].KeyCode = KEY_DOWN;
			keyMap[3].Action = EKA_MOVE_BACKWARD;
			keyMap[3].KeyCode = KEY_KEY_S;

			keyMap[4].Action = EKA_STRAFE_LEFT;
			keyMap[4].KeyCode = KEY_LEFT;
			keyMap[5].Action = EKA_STRAFE_LEFT;
			keyMap[5].KeyCode = KEY_KEY_A;

			keyMap[6].Action = EKA_STRAFE_RIGHT;
			keyMap[6].KeyCode = KEY_RIGHT;
			keyMap[7].Action = EKA_STRAFE_RIGHT;
			keyMap[7].KeyCode = KEY_KEY_D;

			camera = smgr->addCameraSceneNodeFPS(0, 100, speed/1000.0f, -1, keyMap, 8, false, 3.0f);
			camera->setFarValue(farfield);
			camera->setName(cameraName);

			ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(selector, camera, vector3df(10,30,10), vector3df(0,-gravity,0));
			camera->addAnimator(anim);
			anim->drop();
		}

		void setup(char* cameraName, bool bFullScreen, int sizeX, int sizeY, int R, int G, int B)
		{
			strcpy(resourcePath, "");

			E_DRIVER_TYPE driverType = EDT_OPENGL;
			dimension2d<u32> dimension2D = dimension2d<u32>(sizeX, sizeY);

			device = createDevice(driverType, dimension2D, 16, bFullScreen, true, false, &receiver);
			if (device == 0) {comment("Error setup : Device not created",""); return;}

			driver = device->getVideoDriver();
			smgr = device->getSceneManager();

			SizeX = sizeX;
			SizeY = sizeY;

			setCamera(cameraName, 500, 10000, 0);
			camera->setPosition(vector3df(1e-6, 0, 0));

			driver->setFog(SColor(0, 255, 255, 255), EFT_FOG_EXP, 50, 100, 0, true, false);

			device->getCursorControl()->setVisible(false);
			backColor = SColor(255, R, G, B);
		}

		void start()
		{
			int lastFPS = -1;
			u32 last = device->getTimer()->getTime();

			running = true;
			Sleep(1);
			while(device->run())
			{
				//Timer
				const u32 now = device->getTimer()->getTime();
				if (now - last > 100)
				{
					sendMessage(0, strValue1, strValue2);
					last = now;
				}

				//Collisions
				doCollisions();

				//Clicked
				doClicked();

				//Delete nodes as required or other actions
				doActions();

				//Exit
				if (receiver.IsKeyDown(KEY_ESCAPE)) break;

				// draw the 3d scene
				driver->beginScene(true, true, backColor);
				smgr->drawAll();
				driver->endScene();

				int fps = driver->getFPS();
				if (lastFPS != fps)
				{
						stringw tmp("Irrlicht Engine [");
						tmp += driver->getName();
						tmp += L"] fps: ";
						tmp += fps;
						tmp += L" X ";
						tmp += camera->getPosition().X;
						tmp += L" Y ";
						tmp += camera->getPosition().Y;
						tmp += L" Z ";
						tmp += camera->getPosition().Z;

					device->setWindowCaption(tmp.c_str());
					lastFPS = fps;
				}
			}
			/*
			In the end, delete the Irrlicht device.
			*/
			Sleep(100);
			running = false;
			selector->drop();
			device->closeDevice();
			device->drop();
			Sleep(100);
		}

		void setConsoleMessages(bool _consoleMessages)
		{
			consoleMessages = _consoleMessages;
		}

		void addSkybox(char* up, char* dn, char* lf, char* rt, char* ft, char* bk)
		{
			if (running)
			{
				Action action = Action("", ACTION_ADDSKYBOX);
				strcpy(action.up, up);
				strcpy(action.dn, dn);
				strcpy(action.lf, lf);
				strcpy(action.rt, rt);
				strcpy(action.ft, ft);
				strcpy(action.bk, bk);
				addAction(action);
			}
			else
			{
				_addSkybox(up, dn, lf, rt, ft, bk);
			}
		}
		void _addSkybox(char* up, char* dn, char* lf, char* rt, char* ft, char* bk)
		{
			driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, false);
			ISceneNode* skybox = smgr->addSkyBoxSceneNode(
				driver->getTexture(up),
				driver->getTexture(dn),
				driver->getTexture(lf),
				driver->getTexture(rt),
				driver->getTexture(ft),
				driver->getTexture(bk));
			driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, true);
		}

		void addSkydome(char* dome)
		{
			if (running)
			{
				Action action = Action("", ACTION_ADDSKYDOME);
				strcpy(action.dome, dome);
				addAction(action);
			}
			else
			{
				_addSkydome(dome);
			}
		}
		void _addSkydome(char* dome)
		{
			driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, false);
			ISceneNode* skydome = smgr->addSkyDomeSceneNode(driver->getTexture(dome),16,8,0.95f,2.0f);
			driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, true);
		}

		void addCube(char* name)
		{
			if (running)
			{
				Action action = Action(name, ACTION_ADDCUBE);
				addAction(action);
			}
			else
			{
				_addCube(name);
			}
		}
		void _addCube(char* name)
		{
			ISceneNode* node = smgr->addCubeSceneNode();
			if (!node) {comment("Error addCube : Object not created", name); return;}
			node->setMaterialFlag(EMF_LIGHTING, emf_lighting);
			node->setMaterialFlag(EMF_FOG_ENABLE, emf_fog_enable);
			node->setName(name);
		}

		void addSphere(char* name)
		{
			if (running)
			{
				Action action = Action(name, ACTION_ADDSPHERE);
				addAction(action);
			}
			else
			{
				_addSphere(name);
			}
		}
		void _addSphere(char* name)
		{
			ISceneNode* node = smgr->addSphereSceneNode();
			if (!node) {comment("Error addSphere : Object not created", name); return;}
			node->setMaterialFlag(EMF_LIGHTING, emf_lighting);
			node->setMaterialFlag(EMF_FOG_ENABLE, emf_fog_enable);
			node->setName(name);
		}

		void remove(char* name)
		{
			if (running)
			{
				Action action = Action(name, ACTION_REMOVE);
				addAction(action);
			}
			else
			{
				_remove(name);
			}
		}
		void _remove(char* name)
		{
			ISceneNode* node = smgr->getSceneNodeFromName(name);
			if (!node) {comment("Error remove : Object not found", name); return;}
			node->remove();
		}

		void addAnimatedMesh(char* name, char* mesh, int startFrame, int endFrame, int fps)
		{
			if (running)
			{
				Action action = Action(name, ACTION_ADDANIMATEDMESH);
				strcpy(action.mesh, mesh);
				action.startFrame = startFrame;
				action.endFrame = endFrame;
				action.fps = fps;
				addAction(action);
			}
			else
			{
				_addAnimatedMesh(name, mesh, startFrame, endFrame, fps);
			}
		}
		void _addAnimatedMesh(char* name, char* mesh, int startFrame, int endFrame, int fps)
		{
			IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(smgr->getMesh(mesh));
			if (!node) {comment("Error addAnimatedMesh : Object not created", name); return;}
			node->setMaterialFlag(EMF_LIGHTING, emf_lighting);
			node->setMaterialFlag(EMF_FOG_ENABLE, emf_fog_enable);
			node->setName(name);
			node->setFrameLoop(startFrame, endFrame);
			node->setAnimationSpeed(fps);
		}

		void addTerrain(char* name, char* height, char* terrain, char* detail, float gravity, int X, int Y)
		{
			if (running)
			{
				Action action = Action(name, ACTION_ADDTERRAIN);
				strcpy(action.height, height);
				strcpy(action.terrain, terrain);
				strcpy(action.detail, detail);
				action.gravity = gravity;
				action.X = X;
				action.Y = Y;
				addAction(action);
			}
			else
			{
				_addTerrain(name, height, terrain, detail, gravity, X, Y);
			}
		}
		void _addTerrain(char* name, char* height, char* terrain, char* detail, float gravity, int X, int Y)
		{
			ITerrainSceneNode* node = smgr->addTerrainSceneNode(
				height,
				0,					// parent node
				-1,					// node id
				vector3df(-40/2*X, -1000.f, -40/2*Y),		// position
				vector3df(0.f, 0.f, 0.f),		// rotation
				vector3df(40.f, 4.4f, 40.f),	// scale
				SColor ( 255, 255, 255, 255 ),	// vertexColor
				5,					// maxLOD
				ETPS_17,				// patchSize
				//4					// smoothFactor
				0					// smoothFactor
				);
			if (!node) {comment("Error addTerrain : Object not created", name); return;}
			node->setMaterialType(EMT_DETAIL_MAP);
			node->setMaterialFlag(EMF_FOG_ENABLE, emf_fog_enable);
			node->setName(name);

			node->setMaterialTexture(0, driver->getTexture(terrain));
			node->setMaterialTexture(1, driver->getTexture(detail));
	
			node->setMaterialType(EMT_DETAIL_MAP);

			node->scaleTexture(1.0f, 20.0f);

			// create triangle selector for the terrain	
			selector = smgr->createTerrainTriangleSelector(node, 0);
			node->setTriangleSelector(selector);

			// create collision response animator and attach it to the camera
			ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(selector, camera, vector3df(10,30,10), vector3df(0,-gravity,0));
			camera->addAnimator(anim);
			anim->drop();
		}

		void addZip(char* name)
		{
			if (running)
			{
				Action action = Action(name, ACTION_ADDZIP);
				addAction(action);
			}
			else
			{
				_addZip(name);
			}
		}
		void _addZip(char* name)
		{
			device->getFileSystem()->addZipFileArchive(name);
		}

		void addQuakeMesh(char* name, char* mesh, float gravity)
		{
			if (running)
			{
				Action action = Action(name, ACTION_ADDQUAKEMESH);
				strcpy(action.mesh, mesh);
				action.gravity = gravity;
				addAction(action);
			}
			else
			{
				_addQuakeMesh(name, mesh, gravity);
			}
		}
		void _addQuakeMesh(char* name, char* mesh, float gravity)
		{
			IAnimatedMesh* _mesh = smgr->getMesh(mesh);
			if (!_mesh) {comment("Error addQuakeMesh : Mesh not found", name); return;}
			IMeshSceneNode* node = smgr->addOctreeSceneNode(_mesh->getMesh(0), 0, -1, 1024);
			if (!node) {comment("Error addQuakeMesh : Object not created", name); return;}
			node->setPosition(-_mesh->getBoundingBox().getCenter());
			node->setMaterialFlag(EMF_FOG_ENABLE, emf_fog_enable);
			node->setName(name);

			selector = smgr->createOctreeTriangleSelector(node->getMesh(), node, 128);
			if (!selector) {comment("Error addQuakeMesh : Selector not created", name); return;}
			node->setTriangleSelector(selector);
			ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(selector, camera, vector3df(10,30,10), vector3df(0,-gravity,0));
			camera->addAnimator(anim);
			anim->drop();
		}

		void setTexture(char* name, char* texture)
		{
			if (running)
			{
				Action action = Action(name, ACTION_SETTEXTURE);
				strcpy(action.texture, texture);
				addAction(action);
			}
			else
			{
				_setTexture(name, texture);
			}
		}
		void _setTexture(char* name, char* texture)
		{
			ISceneNode* node = smgr->getSceneNodeFromName(name);
			if (!node) {comment("Error setTexture : Object not found", name); return;}
			node->setMaterialTexture(0, driver->getTexture(texture));
			node->setMaterialFlag(EMF_LIGHTING, emf_lighting);
		}

		void setPosition(char* name, float X, float Y, float Z)
		{
			if (running) 
			{
				Action action = Action(name, ACTION_SETPOSITION);
				action.X = X;
				action.Y = Y;
				action.Z = Z;
				addAction(action);
			}
			else
			{
				_setPosition(name, X, Y, Z);
			}
		}
		void _setPosition(char* name, float X, float Y, float Z)
		{
			ISceneNode* node = smgr->getSceneNodeFromName(name);
			if (!node) {comment("Error setPosition : Object not found", name); return;}
			node->setPosition(vector3df(X, Y, Z));
		}

		void getPosition(char* name, float position[3])
		{
			ISceneNode* node = smgr->getSceneNodeFromName(name);
			if (!node) {comment("Error getPosition : Object not found", name); return;}
			vector3df pos = node->getPosition();
			position[0] = pos.X;
			position[1] = pos.Y;
			position[2] = pos.Z;
		}

		void getPosition(float position[3])
		{
			if (!camera) {comment("Error getPosition : Object not found", "Camera"); return;}
			vector3df pos = camera->getPosition();
			position[0] = pos.X;
			position[1] = pos.Y;
			position[2] = pos.Z;
		}

		void getDirection(float direction[3])
		{
			if (!camera) {comment("Error getDirection : Object not found", "Camera"); return;}
			vector3df dir = camera->getTarget()-camera->getPosition();
			float length = dir.getLength();
			direction[0] = dir.X / length;
			direction[1] = dir.Y / length;
			direction[2] = dir.Z / length;
		}

		void setScale(char* name, float X, float Y, float Z)
		{
			if (running) 
			{
				Action action = Action(name, ACTION_SETSCALE);
				action.X = X;
				action.Y = Y;
				action.Z = Z;
				addAction(action);
			}
			else
			{
				_setScale(name, X, Y, Z);
			}
		}
		void _setScale(char* name, float X, float Y, float Z)
		{
			ISceneNode* node = smgr->getSceneNodeFromName(name);
			if (!node) {comment("Error setScale : Object not found", name); return;}
			node->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
			node->setScale(vector3df(X, Y, Z));
		}

		void setRotation(char* name, float X, float Y, float Z)
		{
			if (running) 
			{
				Action action = Action(name, ACTION_SETROTATION);
				action.X = X;
				action.Y = Y;
				action.Z = Z;
				addAction(action);
			}
			else
			{
				_setRotation(name, X, Y, Z);
			}
		}
		void _setRotation(char* name, float X, float Y, float Z)
		{
			ISceneNode* node = smgr->getSceneNodeFromName(name);
			if (!node) {comment("Error setRotation : Object not found", name); return;}
			node->setRotation(vector3df(X, Y, Z));
		}

		float getTerrainHeight(float X, float Z)
		{
			ITerrainSceneNode* node = (ITerrainSceneNode*)smgr->getSceneNodeFromType(ESCENE_NODE_TYPE::ESNT_TERRAIN);
			if (!node) {comment("Error getTerrainHeight : Terrain not found", ""); return 0;}
			return node->getHeight(X, Z) + 2.0; //A little above
		}

        void setVisible(char* name, bool visible)
        {
			if (running) 
			{
				Action action = Action(name, ACTION_SETVISIBLE);
				action.visible = visible;
				addAction(action);
			}
			else
			{
				_setVisible(name, visible);
			}
        }
        void _setVisible(char* name, bool visible)
        {
			ISceneNode* node = smgr->getSceneNodeFromName(name);
			if (!node) {comment("Error setVisible : Object not found", name); return;}
			node->setVisible(visible);
        }

        void animateTo(char* name, float X, float Y, float Z, float time)
        {
			if (running) 
			{
				Action action = Action(name, ACTION_ANIMATETO);
				action.X = X;
				action.Y = Y;
				action.Z = Z;
				action.time = time;
				addAction(action);
			}
			else
			{
				_animateTo(name, X, Y, Z, time);
			}
		}
        void _animateTo(char* name, float X, float Y, float Z, float time)
        {
			ISceneNode* node = smgr->getSceneNodeFromName(name);
			if (!node) {comment("Error animateTo : Object not found", name); return;}
			vector3df startPos = node->getPosition();
			ISceneNodeAnimator* anim = smgr->createFlyStraightAnimator(startPos, vector3df(X, Y, Z), time);
			if (!anim) {comment("Error animateTo : Animator not created", name); return;}
			cleanAnimators(node, anim);
            node->addAnimator(anim);
            anim->drop();
		}

        void animateRelative(char* name, float X, float Y, float Z, float time)
		{
			if (running) 
			{
				Action action = Action(name, ACTION_ANIMATERELATIVE);
				action.X = X;
				action.Y = Y;
				action.Z = Z;
				action.time = time;
				addAction(action);
			}
			else
			{
				_animateRelative(name, X, Y, Z, time);
			}
		}
        void _animateRelative(char* name, float X, float Y, float Z, float time)
		{
			float position[3];
			getPosition(name, position);
            _animateTo(name, position[0] + X, position[1] + Y, position[2] + Z, time);
		}

        void animateOnTerrainTo(char* name, float X, float Z, float time)
		{
			if (running) 
			{
				Action action = Action(name, ACTION_ANIMATEONTERRAINTO);
				action.X = X;
				action.Z = Z;
				action.time = time;
				addAction(action);
			}
			else
			{
				_animateOnTerrainTo(name, X, Z, time);
			}
		}
        void _animateOnTerrainTo(char* name, float X, float Z, float time)
		{
			float Y = getTerrainHeight(X, Z);
            _animateTo(name, X, Y, Z, time);
		}

        void animateOnTerrainRelative(char* name, float X, float Z, float time)
		{
			if (running) 
			{
				Action action = Action(name, ACTION_ANIMATEONTERRAINRELATIVE);
				action.X = X;
				action.Z = Z;
				action.time = time;
				addAction(action);
			}
			else
			{
				_animateOnTerrainRelative(name, X, Z, time);
			}
		}
        void _animateOnTerrainRelative(char* name, float X, float Z, float time)
		{
			float position[3];
			getPosition(name, position);
			float Y = getTerrainHeight(position[0] + X, position[2] + Z);
            _animateTo(name, position[0] + X, Y, position[2] + Z, time);
		}

        void animateRotation(char* name, float X, float Y, float Z)
        {
			if (running) 
			{
				Action action = Action(name, ACTION_ANIMATEROTATION);
				action.X = X;
				action.Y = Y;
				action.Z = Z;
				addAction(action);
			}
			else
			{
				_animateRotation(name, X, Y, Z);
			}
		}
        void _animateRotation(char* name, float X, float Y, float Z)
        {
			ISceneNode* node = smgr->getSceneNodeFromName(name);
			if (!node) {comment("Error animateRotation: Object not found", name); return;}
			ISceneNodeAnimator* anim = smgr->createRotationAnimator(vector3df(X / 100.0, Y / 100.0, Z / 100.0));
			if (!anim) {comment("Error animateRotation : Object not created", name); return;}
			cleanAnimators(node, anim);
            node->addAnimator(anim);
            anim->drop();
		}

        void animateCircle(char* name, float radius, float speed)
        {
			if (running) 
			{
				Action action = Action(name, ACTION_ANIMATECIRCLE);
				action.radius = radius;
				action.speed = speed;
				addAction(action);
			}
			else
			{
				_animateCircle(name, radius, speed);
			}
		}
        void _animateCircle(char* name, float radius, float speed)
        {
			ISceneNode* node = smgr->getSceneNodeFromName(name);
			if (!node) {comment("Error animateCircle: Object not found", name); return;}
			ISceneNodeAnimator* anim = smgr->createFlyCircleAnimator(node->getPosition(), radius, speed / 1000 / 180.0 * pi);
			if (!anim) {comment("Error animateCircle : Object not created", name); return;}
			cleanAnimators(node, anim);
            node->addAnimator(anim);
            anim->drop();
		}

        void removeAnimators(char* name)
        {
			if (running) 
			{
				Action action = Action(name, ACTION_REMOVEANIMATORS);
				addAction(action);
			}
			else
			{
				_removeAnimators(name);
			}
		}
        void _removeAnimators(char* name)
        {
			ISceneNode* node = smgr->getSceneNodeFromName(name);
			if (!node) {comment("Error removeAnimators : Object not found", name); return;}
			node->removeAnimators();
		}

		void setGravity(char* name, float gravity)
		{
			if (running) 
			{
				Action action = Action(name, ACTION_SETGRAVITY);
				action.gravity = gravity;
				addAction(action);
			}
			else
			{
				_setGravity(name, gravity);
			}
		}
		void _setGravity(char* name, float gravity)
		{
			ISceneNode* node = smgr->getSceneNodeFromName(name);
			if (!node) {comment("Error setGravity: Object not found", name); return;}
			if (!selector) {comment("Error setGravity : Object not found", name); return;}

			const aabbox3d<f32>& box = node->getBoundingBox();
			vector3df radius = box.MaxEdge - box.getCenter();
			ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(selector, node, radius, vector3df(0,-gravity,0));
			node->addAnimator(anim);
			anim->drop();
		}

		void setGlow(char* name, int R, int G, int B, float radius)
		{
			if (running) 
			{
				Action action = Action(name, ACTION_SETGLOW);
				action.R = R;
				action.G = G;
				action.B = B;
				action.radius = radius;
				addAction(action);
			}
			else
			{
				_setGlow(name, R, G, B, radius);
			}
		}
		void _setGlow(char* name, int R, int G, int B, float radius)
		{
			ISceneNode* node = smgr->getSceneNodeFromName(name);
			if (!node) {comment("Error setGlow : Object not found", name); return;}
			node->setMaterialFlag(EMF_LIGHTING, false);
			node->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
			node->getMaterial(0).EmissiveColor = SColor(255, R, G, B);
			smgr->addLightSceneNode(node, vector3df(0,0,0), SColor(255, R, G, B), radius);
		}

		void addLight(char* name, float X, float Y, float Z, int R, int G, int B, float radius, char* fireball, char* particlewhite)
		{
			if (running) 
			{
				Action action = Action(name, ACTION_ADDLIGHT);
				action.R = R;
				action.G = G;
				action.B = B;
				action.X = X;
				action.Y = Y;
				action.Z = Z;
				action.radius = radius;
				strcpy(action.fireball, fireball);
				strcpy(action.particlewhite, particlewhite);
				addAction(action);
			}
			else
			{
				_addLight(name, X, Y, Z, R, G, B, radius, fireball, particlewhite);
			}
		}
		void _addLight(char* name, float X, float Y, float Z, int R, int G, int B, float radius, char* fireball, char* particlewhite)
		{
			ISceneNode* light = smgr->addLightSceneNode(0, vector3df(X,Y,Z), SColor(255, R, G, B), radius);
			if (!light) {comment("Error addLight : Object not created", name); return;}
			light->setName(name);

			ISceneNode* bill = smgr->addBillboardSceneNode(light, dimension2d<f32>(30, 30));
			bill->setMaterialFlag(EMF_LIGHTING, false);
			bill->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
			bill->setMaterialTexture(0, driver->getTexture(particlewhite));

			IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(false, light);

			IParticleEmitter* em = ps->createBoxEmitter(
				aabbox3d<f32>(-3,0,-3,3,1,3),
				vector3df(0.0f,0.03f,0.0f),
				20,	25,
				SColor(0,0.5*R,0.5*G,0.5*B), SColor(0,R,G,B),
				400,1100);
			em->setMinStartSize(dimension2d<f32>(15.0f, 20.0f));
			em->setMaxStartSize(dimension2d<f32>(15.0f, 20.0f));
			ps->setEmitter(em);
			em->drop();

			IParticleAffector* paf = ps->createFadeOutParticleAffector();
			ps->addAffector(paf);
			paf->drop();

			ps->setMaterialFlag(EMF_LIGHTING, false);
			ps->setMaterialType(EMT_TRANSPARENT_VERTEX_ALPHA);
			ps->setMaterialTexture(0, driver->getTexture(fireball));
		}

		void ambientLight(int R, int G, int B)
		{
			if (running) 
			{
				Action action = Action("", ACTION_AMBIENTLIGHT);
				action.R = R;
				action.G = G;
				action.B = B;
				addAction(action);
			}
			else
			{
				_ambientLight(R, G, B);
			}
		}
		void _ambientLight(int R, int G, int B)
		{
			smgr->setAmbientLight(SColor(255, R, G, B));
		}

		void addFog(int R, int G, int B, float density)
		{
			if (running) 
			{
				Action action = Action("", ACTION_ADDFOG);
				action.R = R;
				action.G = G;
				action.B = B;
				action.density = density;
				addAction(action);
			}
			else
			{
				_addFog(R, G, B, density);
			}
		}
		void _addFog(int R, int G, int B, float density)
		{
			driver->setFog(SColor(0, R, G, B), EFT_FOG_EXP, 50, 100, density, true, false);
		}

		void setShininess(char* name, float shininess)
		{
			if (running) 
			{
				Action action = Action(name, ACTION_SETSHININESS);
				action.shininess = shininess;
				addAction(action);
			}
			else
			{
				_setShininess(name, shininess);
			}
		}
		void _setShininess(char* name, float shininess)
		{
			ISceneNode* node = smgr->getSceneNodeFromName(name);
			if (!node) {comment("Error setSpecular : Object not found", name); return;}
			node->getMaterial(0).Shininess = shininess;
		}

		void setCameraProperties(float speed, float farfield, float gravity)
		{
			if (running) 
			{
				Action action = Action("", ACTION_SETCAMERAPROPERTIES);
				action.speed = speed;
				action.farfield = farfield;
				action.farfield = gravity;
				addAction(action);
			}
			else
			{
				_setCameraProperties(speed, farfield, gravity);
			}
		}
		void _setCameraProperties(float speed, float farfield, float gravity)
		{
			if (!camera) {comment("Error setCameraProperties : Object not found", "Camera"); return;}
			setCamera((char*)camera->getName(), speed, farfield, gravity);
		}

		void addBillBoard(char* name, float width, float height)
		{
			if (running) 
			{
				Action action = Action(name, ACTION_ADDBILLBOARD);
				action.X = width;
				action.Y = height;
				addAction(action);
			}
			else
			{
				_addBillBoard(name, width, height);
			}
		}
		void _addBillBoard(char* name, float width, float height)
		{
			ISceneNode* node = smgr->addBillboardSceneNode(0, dimension2d<f32>(width, height));
			if (!node) {comment("Error addBillBoard : Object not created", name); return;}
			node->setMaterialFlag(EMF_LIGHTING, emf_lighting);
			node->setMaterialFlag(EMF_FOG_ENABLE, emf_fog_enable);
			node->setName(name);
		}

		char* getType(char* name)
		{
			strcpy(objectType,"none");
			ISceneNode* node = smgr->getSceneNodeFromName(name);
			if (!node) {comment("Error getType : Object not found", name); return objectType;}
			strcpy(objectType, smgr->getSceneNodeTypeName(node->getType()));
			return objectType;
		}

	};
}