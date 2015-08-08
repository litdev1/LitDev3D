// LitDev3DCLR.h

#pragma once

#include "../LitDev3DCPP/LitDev3DCPP.h"

using namespace System;
using namespace System::Text;
using namespace System::Runtime::InteropServices;

namespace LitDev3D
{
	public ref class EngineCLR
	{
	public:
		delegate void MessageDelegate(int value, StringBuilder^ strValue1, StringBuilder^ strValue2);
	private:
		EngineCPP *Engine;
		MessageDelegate^ Message;
		void* delegatePointer;
	public:
		EngineCLR() {}
		EngineCLR(MessageDelegate^ messageDelegate)
		{
			delegatePointer = (void*)Marshal::GetFunctionPointerForDelegate(messageDelegate).ToPointer(); 
			Engine = new EngineCPP(delegatePointer);
		}
		~EngineCLR() {}

		char* String2Char(String^ str1)
		{
			return (char*)Marshal::StringToHGlobalAnsi(str1).ToPointer(); 
		}
		bool wasKeyDown(int key)
		{
			return Engine->wasKeyDown(key);
		}
		void resetAllKeyDown()
		{
			Engine->resetAllKeyDown();
		}

		void setup(String^ name, bool bFullScreen, int sizeX, int sizeY, int R, int G, int B)
		{
			Engine->setup(String2Char(name), bFullScreen, sizeX, sizeY, R, G, B);
		}
        void start()
		{
			Engine->start();
		}
		void setConsoleMessages(bool consoleMessages)
		{
			Engine->setConsoleMessages(consoleMessages);
		}

		void addSkybox(String^ up, String^ dn, String^ lf, String^ rt, String^ ft, String^ bk)
		{
			Engine->addSkybox(String2Char(up), String2Char(dn), String2Char(lf), String2Char(rt), String2Char(ft), String2Char(bk));
		}
		void addSkydome(String^ dome)
		{
			Engine->addSkydome(String2Char(dome));
		}

		void addCube(String^ name)
		{
			Engine->addCube(String2Char(name));
		}
		void addSphere(String^ name)
		{
			Engine->addSphere(String2Char(name));
		}

		void remove(String^ name)
		{
			Engine->remove(String2Char(name));
		}

		void addAnimatedMesh(String^ name, String^ mesh, int startFrame, int endFrame, int fps)
		{
			Engine->addAnimatedMesh(String2Char(name), String2Char(mesh), startFrame, endFrame, fps);
		}

		void addTerrain(String^ name, String^ height, String^ terrain, String^ detail, float gravity, int X, int Y)
		{
			Engine->addTerrain(String2Char(name), String2Char(height), String2Char(terrain), String2Char(detail), gravity, X, Y);
		}

		void addZip(String^ name)
		{
			Engine->addZip(String2Char(name));
		}

		void addQuakeMesh(String^ name, String^ mesh, float gravity)
		{
			Engine->addQuakeMesh(String2Char(name), String2Char(mesh), gravity);
		}

		void setTexture(String^ name, String^ texture)
		{
			Engine->setTexture(String2Char(name), String2Char(texture));
		}

		void setPosition(String^ name, float X, float Y, float Z)
		{
			Engine->setPosition(String2Char(name), X, Y, Z);
		}

		String^ getPosition(String^ name)
		{
			float position[3];
			Engine->getPosition(String2Char(name), position);
			String^ pos = "";
			for (int i = 0; i < 3; i++)
			{
				pos += (i+1).ToString()+"="+position[i].ToString()+";";
			}
			return pos;
		}

		String^ getDirection()
		{
			float direction[3];
			Engine->getDirection(direction);
			String^ dir = "";
			for (int i = 0; i < 3; i++)
			{
				dir += (i+1).ToString()+"="+direction[i].ToString()+";";
			}
			return dir;
		}

		String^ getPosition()
		{
			float position[3];
			Engine->getPosition(position);
			String^ pos = "";
			for (int i = 0; i < 3; i++)
			{
				pos += (i+1).ToString()+"="+position[i].ToString()+";";
			}
			return pos;
		}

		void setScale(String^ name, float X, float Y, float Z)
		{
			Engine->setScale(String2Char(name), X, Y, Z);
		}

		void setRotation(String^ name, float X, float Y, float Z)
		{
			Engine->setRotation(String2Char(name), X, Y, Z);
		}

		float getTerrainHeight(float X, float Z)
		{
			return Engine->getTerrainHeight(X, Z);
		}

        void setVisible(String^ name, bool visible)
        {
            Engine->setVisible(String2Char(name), visible);
        }

        void animateTo(String^ name, float X, float Y, float Z, float time)
		{
            Engine->animateTo(String2Char(name), X, Y, Z, time);
		}

        void animateRelative(String^ name, float X, float Y, float Z, float time)
		{
            Engine->animateRelative(String2Char(name), X, Y, Z, time);
		}

        void animateOnTerrainTo(String^ name, float X, float Z, float time)
		{
            Engine->animateOnTerrainTo(String2Char(name), X, Z, time);
		}

        void animateOnTerrainRelative(String^ name, float X, float Z, float time)
		{
            Engine->animateOnTerrainRelative(String2Char(name), X, Z, time);
		}

        void animateRotation(String^ name, float X, float Y, float Z)
        {
            Engine->animateRotation(String2Char(name), X, Y, Z);
		}

        void animateCircle(String^ name, float radius, float speed)
		{
            Engine->animateCircle(String2Char(name), radius, speed);
		}

        void removeAnimators(String^ name)
        {
            Engine->removeAnimators(String2Char(name));
		}

		void setGravity(String^ name, float gravity)
		{
            Engine->setGravity(String2Char(name), gravity);
		}

        void setGlow(String^ name, int R, int G, int B, float radius)
        {
            Engine->setGlow(String2Char(name), R, G, B, radius);
		}

        void addLight(String^ name, float X, float Y, float Z, int R, int G, int B, float radius, String^ fireball, String^ particlewhite)
        {
            Engine->addLight(String2Char(name), X, Y, Z, R, G, B, radius, String2Char(fireball), String2Char(particlewhite));
		}

		void ambientLight(int R, int G, int B)
		{
			Engine->ambientLight(R, G, B);
		}

		void addFog(int R, int G, int B, float density)
		{
			Engine->addFog(R, G, B, density);
		}

		void setShininess(String^ name, float shininess)
		{
            Engine->setShininess(String2Char(name), shininess);
		}

		void setCameraProperties(float speed, float farfield, float gravity)
		{
            Engine->setCameraProperties(speed, farfield, gravity);
		}

		void addBillBoard(String^ name, float width, float height)
		{
            Engine->addBillBoard(String2Char(name), width, height);
		}

		String^ getType(String^ name)
		{
			return gcnew String(Engine->getType(String2Char(name)));
		}
	};
}
