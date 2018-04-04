#pragma once
#include <vector>
#include <cmath>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "structures.h"


const float RADIAN = 0.017453292f;
const float PI = 3.141592f;
const float G = 9.81f;



#define MAX_MATERIALS 255

class cObj {
  private:
    std::vector<vertex> vertices;
    std::vector<vertex> texcoords;
    std::vector<vertex> normals;
    std::vector<vertex> parameters;
    std::vector<face> faces;
	unsigned int materials_count;
	material* materials[MAX_MATERIALS];
	int material_index;
    GLuint list;
  protected:
  public:
    cObj(std::string path, std::string filename);
    ~cObj();
	int parseMtlFile(std::string path, std::string filename);
	void setMaterialDefaults(material *mtl);
    void compileList();
    void render();
};