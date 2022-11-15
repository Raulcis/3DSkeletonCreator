
//
//  Starter file for Project 3 - Skeleton Builder
//
//  This file includes functionality that supports selection and translate/rotation
//  of scene objects using the mouse.
//
//  Modifer keys for rotatation are x, y and z keys (for each axis of rotation)
//
//  (c) Kevin M. Smith  - 24 September 2018
//   @Modified: Raul Cisneros
//   @Date:   11 / 16 / 2021
//

#include "ofMain.h"
#include "box.h"
#include "Primitives.h"



class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		static void drawAxis(glm::mat4 transform = glm::mat4(1.0), float len = 1.0);
		bool mouseToDragPlane(int x, int y, glm::vec3 &point);
		void printChannels(SceneObject *);
		bool objSelected() { return (selected.size() ? true : false ); };
		void deleteJoint();
		void saveToFile();
		void openFile(string filename1);
		
		// Lights
		//
		ofLight light1;
	
		// Cameras
		//
		ofEasyCam  mainCam;
		ofCamera sideCam;
		ofCamera topCam;
		ofCamera  *theCam;    // set to current camera either mainCam or sideCam

		// Materials
		//
		ofMaterial material;


		// scene components
		//
		vector<SceneObject *> scene;
		vector<SceneObject *> selected;
		ofPlanePrimitive plane;

		// state
		bool bDrag = false;
		bool bHide = true;
		bool bAltKeyDown = false;
		bool bRotateX = false;
		bool bRotateY = false;
		bool bRotateZ = false;
		glm::vec3 lastPoint;
		glm::vec3 mousePoint;

		Sphere sphereParent;
		Sphere sphereChild;
		Sphere sphereMoon;

		float xval = 0.0;
		int jointCount = 0;
		int one = 1;
		int sceneCount = 1;
		int readInScene = 1;

		Joint* holdJoint;
		vector<Joint*> jointVec;

		SceneObject* holdParent;
		SceneObject* holdChild;

		ofstream myfile;
		ifstream infile;
		string filename;
		string readLine;
		string ignorecreate,ignorejoint,ignorerotate,ignoretrans,ignoreparent;
		string jointName;
		string parentName;
		string rotationVec, translationVec;
		string x, y, z;
		float X, Y, Z;
		string hold;
		glm::vec3 jointRotation;
		glm::vec3 jointPosition;
		bool readFile = false;

		float xpos;
		float ypos;
		string namepar;
};