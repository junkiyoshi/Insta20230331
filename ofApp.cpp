#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openframeworks");

	ofBackground(0);
	ofSetLineWidth(2);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
}

//--------------------------------------------------------------
void ofApp::update() {

	this->mesh_list.clear();

	int span = 20;
	int radius = 90;
	ofColor color;

	while (this->log_list.size() < 2000) {

		int deg_base = ofRandom(360);
		int deg = deg_base / 20 * 20;

		vector<glm::vec2> log;
		auto location = glm::vec2(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD));
		log.push_back(location);
		auto next = glm::vec2((radius - 10) * cos(deg * DEG_TO_RAD), (radius - 10) * sin(deg * DEG_TO_RAD));

		auto distance = location - next;
		distance *= 2;

		auto future = location + distance * 30;
		auto random_deg = ofRandom(360);
		future += glm::vec2(120 * cos(random_deg * DEG_TO_RAD), 120 * sin(random_deg * DEG_TO_RAD));
		auto future_distance = future - location;

		this->log_list.push_back(log);
		this->velocity_list.push_back(glm::normalize(future_distance) * glm::length(distance) * ofRandom(0.25, 0.6));
		color.setHsb(ofMap(deg_base, 0, 360, 0, 255), 255, 255);
		this->color_list.push_back(ofColor(color, 64));
	}

	for (int i = this->log_list.size() - 1; i > -1; i--) {

		auto location = this->log_list[i][this->log_list[i].size() - 1];

		auto deg = atan2(this->velocity_list[i].y, this->velocity_list[i].x) * RAD_TO_DEG;
		deg += ofMap(ofNoise(glm::vec3(location * 0.005, ofGetFrameNum() * 0.01)), 0, 1, -30, 30);
		auto len = glm::length(this->velocity_list[i]);
		location += glm::vec2(len * cos(deg * DEG_TO_RAD), len * sin(deg * DEG_TO_RAD));

		if (glm::distance(glm::vec2(), this->log_list[i][this->log_list[i].size() - 1]) > 1300) {

			this->log_list.erase(this->log_list.begin() + i);
			this->velocity_list.erase(this->velocity_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);

			continue;
		}

		this->log_list[i].push_back(location);
		this->velocity_list[i] *= 1.01;
	}

	for (int i = 0; i < this->log_list.size(); i++) {

		ofMesh mesh;
		mesh.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);

		for (int k = 0; k < this->log_list[i].size(); k++) {

			mesh.addVertex(glm::vec3(this->log_list[i][k], 0));
			mesh.addColor(this->color_list[i]);

			if (k > 0) {

				mesh.addIndex(mesh.getNumVertices() - 1);
				mesh.addIndex(mesh.getNumVertices() - 2);
			}
		}

		this->mesh_list.push_back(mesh);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);

	for (auto& mesh : this->mesh_list) {

		mesh.drawWireframe();
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}