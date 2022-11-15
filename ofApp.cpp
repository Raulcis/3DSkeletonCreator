
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


#include "ofApp.h"



//--------------------------------------------------------------
//
void ofApp::setup() {

	ofSetBackgroundColor(ofColor::black);
	ofEnableDepthTest();
	mainCam.setDistance(15);
	mainCam.setNearClip(.1);
	
	sideCam.setPosition(40, 0, 0);
	sideCam.lookAt(glm::vec3(0, 0, 0));
	topCam.setNearClip(.1);
	topCam.setPosition(0, 16, 0);
	topCam.lookAt(glm::vec3(0, 0, 0));
	ofSetSmoothLighting(true);


	// setup one point light
	//
	light1.enable();
	light1.setPosition(5, 5, 0);
	light1.setDiffuseColor(ofColor(255.f, 255.f, 255.f));
	light1.setSpecularColor(ofColor(255.f, 255.f, 255.f));

	theCam = &mainCam;

	//  create a scene consisting of a ground plane with 2x2 blocks
	//  arranged in semi-random positions, scales and rotations
	//
	// ground plane
	//
	scene.push_back(new Plane(glm::vec3(0, -2, 0), glm::vec3(0, 1, 0)));   

	//add object to seen
	sphereChild.diffuseColor = ofColor::blue;
	sphereMoon.diffuseColor = ofColor::pink;
	sphereChild.position = (glm::vec3(9, 0, 0));
	sphereMoon.position = (glm::vec3(3, 0, 0));
	sphereMoon.radius = .3;
	sphereParent.radius = 3;

	/*
	scene.push_back(&sphereParent);
	scene.push_back(&sphereChild);
	scene.push_back(&sphereMoon);
	sphereParent.addChild(&sphereChild);
	sphereChild.addChild(&sphereMoon);
	*/
}

 
//--------------------------------------------------------------
void ofApp::update(){

//	float scaleval = .2 * glm::sin(xval);
//	sphereParent.rotation += glm::vec3(0, 1.0, 0);
//	sphereChild.rotation += glm::vec3(0, 5.0, 0);
//	sphereChild.scale += glm::vec3(scaleval, scaleval, scaleval);
//	scaleval += 0.5;
}

//--------------------------------------------------------------
void ofApp::draw(){

	theCam->begin();
	ofNoFill();
	drawAxis();
	ofEnableLighting();

	//  draw the objects in scene
	//
	material.begin();
	ofFill();
	
	for (int i = 0; i < scene.size(); i++) {
		if (objSelected() && scene[i] == selected[0])
			ofSetColor(ofColor::white);
		else ofSetColor(scene[i]->diffuseColor);
		scene[i]->draw();
	}

	material.end();
	ofDisableLighting();
	theCam->end();
}

// 
// Draw an XYZ axis in RGB at transform
//
void ofApp::drawAxis(glm::mat4 m, float len) {

	ofSetLineWidth(1.0);

	// X Axis
	ofSetColor(ofColor(255, 0, 0));
	ofDrawLine(glm::vec3(m*glm::vec4(0, 0, 0, 1)), glm::vec3(m*glm::vec4(len, 0, 0, 1)));


	// Y Axis
	ofSetColor(ofColor(0, 255, 0));
	ofDrawLine(glm::vec3(m*glm::vec4(0, 0, 0, 1)), glm::vec3(m*glm::vec4(0, len, 0, 1)));

	// Z Axis
	ofSetColor(ofColor(0, 0, 255));
	ofDrawLine(glm::vec3(m*glm::vec4(0, 0, 0, 1)), glm::vec3(m*glm::vec4(0, 0, len, 1)));
}

// print C++ code for obj tranformation channels. (for debugging);
//
void ofApp::printChannels(SceneObject *obj) {
	cout << "position = glm::vec3(" << obj->position.x << "," << obj->position.y << "," << obj->position.z << ");" << endl;
	cout << "rotation = glm::vec3(" << obj->rotation.x << "," << obj->rotation.y << "," << obj->rotation.z << ");" << endl;
	cout << "scale = glm::vec3(" << obj->scale.x << "," << obj->scale.y << "," << obj->scale.z << ");" << endl;
}

void ofApp::deleteJoint(){

	if (objSelected() == true) {
		for (int i = 0; i < scene.size(); i++) {
			
			if (objSelected() && scene[i] == selected[0]) {
				if (i + one >= scene.size()) {
					scene.erase(scene.begin() + i);
				}
				else {

					if (scene[i]->parent != NULL) {
						if (i - one != -one) {
							scene[i - one]->addChild(scene[i + one]);
						}
						scene.erase(scene.begin() + i);
					}
					else if (scene[i]->parent == NULL){
						scene.erase(scene.begin() + i);
					}
				}
			}
		}
	}
}

void ofApp::saveToFile(){
	
	myfile.open("test.txt");

	for (int i = 1; i < scene.size(); i++) {
		if (scene[i]->parent > 0) {
			myfile << "create -joint " << scene[i]->name << " -rotate " << "<" << scene[i]->rotation << ">" << " -translate "  << "<" << scene[i]->position << ">" << " -parent " << scene[i]->parent->name << "\n";
		}
		else {
			myfile << "create -joint " << scene[i]->name << " -rotate " << "<" << scene[i]->rotation << ">" << " -translate " << scene[i]->position << "\n";
		}
	}

	myfile.close();
}
void ofApp::openFile(string filename1) {
	infile.open(filename1);
	//try get line method forwhile loop
	while (!infile.eof()) {
		if (infile.eof()) break;

		if (readInScene <= 1) {

			getline(infile, readLine);

			stringstream linestream(readLine); // converts the strings read from the file into string stream

			linestream >> ignorecreate >> ignorejoint >> jointName >> ignorerotate;
			linestream >> x >> y >> z;
			for (int i = 0; i < x.size(); i++) {
				if (x[i] != '<' && x[i] != ',') {
					hold = hold + x[i];
				}
			}

			stringstream linestreamXJoint0(hold);

			linestreamXJoint0 >> X;

			hold.clear();

			for (int i = 0; i < y.size(); i++) {
				if (y[i] != '<' && y[i] != ',') {
					hold = hold + y[i];
				}
			}

			stringstream linestreamYJoint0(hold);

			linestreamYJoint0 >> Y;

			hold.clear();

			for (int i = 0; i < z.size(); i++) {
				if (z[i] != '<' && z[i] != ',') {
					hold = hold + z[i];
				}
			}

			stringstream linestreamZJoint0(hold);

			linestreamZJoint0 >> Z;

			hold.clear();

			jointRotation = glm::vec3(X, Y, Z);

			linestream >> ignoretrans;

			linestream >> x >> y >> z;

			for (int i = 0; i < x.size(); i++) {
				if (x[i] != '<' && x[i] != ',') {
					hold = hold + x[i];
				}
			}

			stringstream linestreamx(hold);

			linestreamx >> X;

			hold.clear();

			for (int i = 0; i < y.size(); i++) {
				if (y[i] != '<' && y[i] != ',') {
					hold = hold + y[i];
				}
			}

			stringstream linestreamy(hold);

			linestreamy >> Y;

			hold.clear();

			for (int i = 0; i < z.size(); i++) {
				if (z[i] != '>' && z[i] != ',') {
					hold = hold + z[i];
				}
			}

		//	cout << hold << endl;
			stringstream linestreamz(hold);

			linestreamz >> Z;
		//	cout << X << " " << Y << " " << Z << endl;

			hold.clear();

			jointPosition = glm::vec3(X, Y, Z);

			holdJoint = new Joint(jointPosition, .4, ofColor::blue);
			holdJoint->rotation = jointRotation;

			scene.push_back(holdJoint);
			scene[readInScene]->name = jointName;
			readInScene++;

		//	cout << ignorecreate << " " << ignorejoint << " " << jointName << " " << ignorerotate << jointRotation << " " << ignoretrans << " " <<  jointPosition <<endl;
		}
		else {
			
			//	cout << readLine << endl;
			infile >> ignorecreate;
			//	cout << ignorecreate << endl;
			infile >> ignorejoint;
			//	cout << ignorejoint << endl;
			infile >> jointName;
			//	cout << jointName << endl;
			infile >> ignorerotate;
			//	cout << ignorerotate << endl;

			infile >> x >> y >> z;

			for (int i = 0; i < x.size(); i++) {
				if (x[i] != '<' && x[i] != ',') {
					hold = hold + x[i];
				}
			}

			stringstream linestreamX(hold);

			linestreamX >> X;

			hold.clear();

			for (int i = 0; i < y.size(); i++) {
				if (y[i] != '<' && y[i] != ',') {
					hold = hold + y[i];
				}
			}

			stringstream linestreamY(hold);

			linestreamY >> Y;

			hold.clear();

			for (int i = 0; i < z.size(); i++) {
				if (z[i] != '<' && z[i] != ',') {
					hold = hold + z[i];
				}
			}

			stringstream linestreamZ(hold);

			linestreamZ >> Z;

			hold.clear();

			jointRotation = glm::vec3(X, Y, Z);

			infile >> ignoretrans;

			infile >> x >> y >> z;

			for (int i = 0; i < x.size(); i++) {
				if (x[i] != '<' && x[i] != ',') {
					hold = hold + x[i];
				}
			}

			stringstream linestreamXX(hold);

			linestreamXX >> X;

			hold.clear();

			for (int i = 0; i < y.size(); i++) {
				if (y[i] != '<' && y[i] != ',') {
					hold = hold + y[i];
				}
			}

			stringstream linestreamYY(hold);

			linestreamYY >> Y;

			hold.clear();

			for (int i = 0; i < z.size(); i++) {
				if (z[i] != '>' && z[i] != ',') {
					hold = hold + z[i];
				}
			}

			stringstream linestreamZZ(hold);

			linestreamZZ >> Z;

			hold.clear();

			jointPosition = glm::vec3(X, Y, Z);

			infile >> ignoreparent;

			infile >> parentName;

			//	cout << parentName << endl;

			//	cout << ignorecreate << " " << ignorejoint << " " << jointName << " " << ignorerotate << x << y << z << " " << ignoretrans << " " << ignoreparent << " " << parentName << endl;

			holdJoint = new Joint(jointPosition, .4, ofColor::blue);
			//	holdJoint->name = jointName;
			holdJoint->rotation = jointRotation;

			scene.push_back(holdJoint);
			scene[readInScene]->name = jointName;
			if (readInScene > 1) {

				if (parentName.size() > 0) {
					for (int i = 0; i < scene.size(); i++) {
						if (parentName == scene[i]->name) {
							scene[i]->addChild(holdJoint);
						}
					}
				}
				parentName.empty();
			}
			readInScene++;
		}
		
	}
	infile.close();
	readFile = true;

}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){

	switch (key) {
	case OF_KEY_ALT:
		bAltKeyDown = false;
		mainCam.disableMouseInput();
		break;
	case 'x':
		bRotateX = false;
		break;
	case 'y':
		bRotateY = false;
		break;
	case 'z':
		bRotateZ = false;
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key) {
	case 'C':
	case 'c':
		if (mainCam.getMouseInputEnabled()) mainCam.disableMouseInput();
		else mainCam.enableMouseInput();
		break;
	case 'd':
		deleteJoint();
		cout << "deleted selected joint" << endl;
		break;
	case 'b':
		break;
	case 'f':
		ofToggleFullscreen();
		break;
	case 'h':
		bHide = !bHide;
		break;
	case 'i':
		break;
	case 'j':

		if (readFile == true) {
			sceneCount = scene.size();
			jointCount = scene.size();

		}
		if (objSelected() == false) {
			scene.push_back(new Joint(mousePoint, .4, ofColor::hotPink));
			scene[sceneCount]->name = "Joint" + std::to_string(jointCount);
			jointCount++;
			sceneCount++;
		}
		else {
			
			if (jointCount == 2) {

				holdJoint = new Joint(mousePoint, .4, ofColor::blue);
				scene.push_back(holdJoint);
				scene[sceneCount]->name = "Joint" + std::to_string(jointCount);
				jointCount++;
				sceneCount++;
			}
			else {
				holdJoint = new Joint(mousePoint, .4, ofColor::hotPink);
				scene.push_back(holdJoint);
				scene[sceneCount]->name = "Joint" + std::to_string(jointCount);
				jointCount++;
				sceneCount++;
			}
			for (int i = 0; i < scene.size(); i++) {
				if (objSelected() && scene[i] == selected[0]) {
					holdJoint->position = mousePoint - scene[i]->position ;
					scene[i]->addChild(holdJoint);
				}
			}
			
		}
		break;
	case 'n':
		break;
	case 'p':
		if (objSelected()) printChannels(selected[0]);
		break;
	case 's':
		saveToFile();
		cout << "File created" << endl;
		break;
	case 'x':
		bRotateX = true;
		break;
	case 'y':
		bRotateY = true;
		break;
	case 'z':
		bRotateZ = true;
		break;
	case OF_KEY_F1: 
		theCam = &mainCam;
		break;
	case OF_KEY_F2:
		theCam = &sideCam;
		break;
	case OF_KEY_F3:
		theCam = &topCam;
		break;
	case OF_KEY_ALT:
		bAltKeyDown = true;
		if (!mainCam.getMouseInputEnabled()) mainCam.enableMouseInput();
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	xpos = ofGetMouseX();
	ypos = ofGetMouseY();
	mouseToDragPlane(xpos, ypos, mousePoint);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

	if (objSelected() && bDrag) {
		glm::vec3 point; 
		mouseToDragPlane(x, y, point);
		if (bRotateX) {
			selected[0]->rotation += glm::vec3((point.x - lastPoint.x) * 20.0, 0, 0);
		}
		else if (bRotateY) {
			selected[0]->rotation += glm::vec3(0, (point.x - lastPoint.x) * 20.0, 0);
		}
		else if (bRotateZ) {
			selected[0]->rotation += glm::vec3(0, 0, (point.x - lastPoint.x) * 20.0);
		}
		else {
			selected[0]->position += (point - lastPoint);
		}
		lastPoint = point;
	}

}

//  This projects the mouse point in screen space (x, y) to a 3D point on a plane
//  normal to the view axis of the camera passing through the point of the selected object.
//  If no object selected, the plane passing through the world origin is used.
//
bool ofApp::mouseToDragPlane(int x, int y, glm::vec3 &point) {
	glm::vec3 p = theCam->screenToWorld(glm::vec3(x, y, 0));
	glm::vec3 d = p - theCam->getPosition();
	glm::vec3 dn = glm::normalize(d);

	float dist;
	glm::vec3 pos;
	if (objSelected()) {
		pos = selected[0]->position;
	}
	else pos = glm::vec3(0, 0, 0);
	if (glm::intersectRayPlane(p, dn, pos, glm::normalize(theCam->getZAxis()), dist)) {
		point = p + dn * dist;
		return true;
	}
	return false;
}

//--------------------------------------------------------------
//
// Provides functionality of single selection and if something is already selected,
// sets up state for translation/rotation of object using mouse.
//
void ofApp::mousePressed(int x, int y, int button){

	// if we are moving the camera around, don't allow selection
	//
	if (mainCam.getMouseInputEnabled()) return;

	// clear selection list
	//
	selected.clear();

	//
	// test if something selected
	//
	vector<SceneObject *> hits;

	glm::vec3 p = theCam->screenToWorld(glm::vec3(x, y, 0));
	glm::vec3 d = p - theCam->getPosition();
	glm::vec3 dn = glm::normalize(d);

	// check for selection of scene objects
	if(scene.size() != 0){
		for (int i = 0; i < scene.size(); i++) {

			glm::vec3 point, norm;

			//  We hit an object
			//
			if (scene[i]->isSelectable && scene[i]->intersect(Ray(p, dn), point, norm)) {
				hits.push_back(scene[i]);
			}
		}
	}


	// if we selected more than one, pick nearest
	//
	SceneObject *selectedObj = NULL;
	if (hits.size() > 0) {
		selectedObj = hits[0];
		float nearestDist = std::numeric_limits<float>::infinity();
		for (int n = 0; n < hits.size(); n++) {
			float dist = glm::length(hits[n]->position - theCam->getPosition());
			if (dist < nearestDist) {
				nearestDist = dist;
				selectedObj = hits[n];
			}	
		}
	}
	if (selectedObj) {
		selected.push_back(selectedObj);
		bDrag = true;
		mouseToDragPlane(x, y, lastPoint);
	}
	else {
		selected.clear();
		mouseToDragPlane(x, y, lastPoint);
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	bDrag = false;

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
	//used to be able to drag and drop a file
	if (dragInfo.files.size() > 0) {
		scene.clear();
		scene.push_back(new Plane(glm::vec3(0, -2, 0), glm::vec3(0, 1, 0)));
		 sceneCount = 1;
		 readInScene = 1;
		 jointCount = 0;
		openFile(dragInfo.files[0]);
	}
}

