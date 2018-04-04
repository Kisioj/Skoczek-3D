#include "cObj.h"

std::string pad(int x) {
	std::stringstream s;
	if      (x < 10)   s << "00" << x;
	else if (x < 100)  s << "0"  << x;
	else if (x < 1000) s << ""   << x;
	return s.str();
}

std::string number_format(int x) {
	std::stringstream s;
	std::vector<int> sep; int t; bool flag = false;
	if (x < 0) { flag = true; x = -x; }
	while (x > 1000) {
		t = x % 1000;
		sep.push_back(t);
		x = x / 1000;
	};
	sep.push_back(x);
	for (size_t i = sep.size() - 1; i > 0; i--) {
		if (i == sep.size() - 1) s << (flag ? "-" : "") << sep[i] << ",";
		else s << pad(sep[i]) << ",";
	}
	if (sep.size() > 1)
		s << pad(sep[0]);
	else
		s << (flag ? "-" : "") << sep[0];
	return s.str();
}

cObj::cObj(std::string path, std::string filename) {
	this->materials_count = 0;

    std::ifstream ifs( (path+filename).c_str(), std::ifstream::in);
    std::string line, key;

	unsigned int i = -1; //material_index

    while (ifs.good() && !ifs.eof() && std::getline(ifs, line)) {

        key = "";
        std::stringstream stringstream(line);
        stringstream >> key >> std::ws;
         
        if (key == "v") { // vertex
            vertex v; float x;
            while (!stringstream.eof()) {
                stringstream >> x >> std::ws;
                v.v.push_back(x);
            }
            vertices.push_back(v);
        } else if (key == "vp") { // parameter
            vertex v; float x;
            while (!stringstream.eof()) {
                stringstream >> x >> std::ws;
                v.v.push_back(x);
            }
            parameters.push_back(v);
        } else if (key == "vt") { // texture coordinate
            vertex v; float x;
            while (!stringstream.eof()) {
                stringstream >> x >> std::ws;
                v.v.push_back(x);
            }
            texcoords.push_back(v);
        } else if (key == "vn") { // normal
            vertex v; float x;
            while (!stringstream.eof()) {
                stringstream >> x >> std::ws;
                v.v.push_back(x);
            }
            v.normalize();
            normals.push_back(v);
        } else if (key == "f") { // face
            face f; int v, t, n;
            while (!stringstream.eof()) {
                stringstream >> v >> std::ws;
                f.vertex.push_back(v-1);
                if (stringstream.peek() == '/') {
                    stringstream.get();
                    if (stringstream.peek() == '/') {
                        stringstream.get();
                        stringstream >> n >> std::ws;
                        f.normal.push_back(n-1);
                    } else {
                        stringstream >> t >> std::ws;
                        f.texture.push_back(t-1);
                        if (stringstream.peek() == '/') {
                            stringstream.get();
                            stringstream >> n >> std::ws;
                            f.normal.push_back(n-1);
                        }
                    }
                }
            }
			f.material = i;
            faces.push_back(f);
		} else if (key == "usemtl") { // usemtl
			string s;
            while (!stringstream.eof()) {
                stringstream >> s >> std::ws;
            }
			for(unsigned int j=0; j<materials_count; j++){
				if(s == materials[j]->name){
					i = j;
					break;
				}
			}
		} else if (key == "mtllib") { // mtllib
			string s;
            while (!stringstream.eof()) {
                stringstream >> s >> std::ws;
            }
			parseMtlFile(path,s);

        } else {
 
        }
    }
    ifs.close();
    /*std::cout << "               Name: " << filename << std::endl;
    std::cout << "           Vertices: " << number_format(int(vertices.size())) << std::endl;
    std::cout << "         Parameters: " << number_format(int(parameters.size())) << std::endl;
    std::cout << "Texture Coordinates: " << number_format(int(texcoords.size())) << std::endl;
    std::cout << "            Normals: " << number_format(int(normals.size())) << std::endl;
    std::cout << "              Faces: " << number_format(int(faces.size())) << std::endl;
	std::cout << "          Materials: " << number_format(this->materials_count) << std::endl << std::endl;
	std::cout << "          Material IDs: \n";
	for(unsigned int k=0;k<materials_count;k++){
		std::cout << k <<"# " << materials[k]->texture.getTextureID() << "\n";
	}*/

    list = glGenLists(1);
    compileList();
    vertices.clear();
    texcoords.clear();
    normals.clear();
    faces.clear();
}

int cObj::parseMtlFile(std::string path, std::string filename) {
	std::ifstream ifs( (path+filename).c_str(), std::ifstream::in);
    std::string line, key;

	unsigned int i = -1; //material mTextureID

    while (ifs.good() && !ifs.eof() && std::getline(ifs, line)) {

        key = "";
        std::stringstream stringstream(line);
        stringstream >> key >> std::ws;
        
		//start material
        if (key == "newmtl") {
			materials_count++;
			i++;
			materials[i] = new material;
			//material m;
			//materials.push_back(m);
            while (!stringstream.eof()) {
				stringstream >> materials[i]->name >> std::ws;
            }
		//ambient
		}else if(key == "Ka"){
			while (!stringstream.eof()) {
				stringstream >> materials[i]->amb[0] >> materials[i]->amb[1] >> materials[i]->amb[2] >> std::ws;
            }
		//diff
		}else if(key == "Kd"){
			while (!stringstream.eof()) {
				stringstream >> materials[i]->diff[0] >> materials[i]->diff[1] >> materials[i]->diff[2] >> std::ws;
            }
		//specular
		}else if(key == "Ks"){
			while (!stringstream.eof()) {
				stringstream >> materials[i]->spec[0] >> materials[i]->spec[1] >> materials[i]->spec[2] >> std::ws;
            }
		//shiny
		}else if(key == "Ns"){
			while (!stringstream.eof()) {
				stringstream >> materials[i]->shiny >> std::ws;
            }
		//transparent
		}else if(key == "d"){
			while (!stringstream.eof()) {
				stringstream >> materials[i]->trans >> std::ws;
            }
		//reflection
		}else if(key == "r"){
			while (!stringstream.eof()) {
				stringstream >> materials[i]->reflect >> std::ws;
            }
		//glossy
		}else if(key == "sharpness"){
			while (!stringstream.eof()) {
				stringstream >> materials[i]->glossy >> std::ws;
            }
		//refract index
		}else if(key == "Ni"){
			while (!stringstream.eof()) {
				stringstream >> materials[i]->refract_index >> std::ws;
            }
		// texture map
		}else if(key == "map_Kd"){
			while (!stringstream.eof()) {
				stringstream >> materials[i]->texture_filename >> std::ws;
            }
			materials[i]->texture.loadTextureFromFile(path+materials[i]->texture_filename);
		}
	}
	ifs.close();
	return 1;
}

void cObj::setMaterialDefaults(material *mtl){
	mtl->amb[0] = 0.2f;
	mtl->amb[1] = 0.2f;
	mtl->amb[2] = 0.2f;
	mtl->diff[0] = 0.8f;
	mtl->diff[1] = 0.8f;
	mtl->diff[2] = 0.8f;
	mtl->spec[0] = 1.0f;
	mtl->spec[1] = 1.0f;
	mtl->spec[2] = 1.0f;
	mtl->reflect = 0.0f;
	mtl->trans = 1;
	mtl->glossy = 98;
	mtl->shiny = 0;
	mtl->refract_index = 1;
	mtl->texture_filename[0] = '\0';
}

void cObj::compileList() {
    glNewList(list, GL_COMPILE);
    for (unsigned int i = 0; i < faces.size(); i++) {
        if (faces[i].vertex.size() == 3) { // triangle
            if (faces[i].normal.size() == 3) { // with normals
				
                glBegin(GL_TRIANGLES);
                glNormal3f(normals[faces[i].normal[0]].v[0], normals[faces[i].normal[0]].v[1], normals[faces[i].normal[0]].v[2]);
				glTexCoord2f(texcoords[faces[i].texture[0]].v[0],texcoords[faces[i].texture[0]].v[1]);
                glVertex3f(vertices[faces[i].vertex[0]].v[0], vertices[faces[i].vertex[0]].v[1], vertices[faces[i].vertex[0]].v[2]);

                glNormal3f(normals[faces[i].normal[1]].v[0], normals[faces[i].normal[1]].v[1], normals[faces[i].normal[1]].v[2]);
				glTexCoord2f(texcoords[faces[i].texture[1]].v[0],texcoords[faces[i].texture[1]].v[1]);
                glVertex3f(vertices[faces[i].vertex[1]].v[0], vertices[faces[i].vertex[1]].v[1], vertices[faces[i].vertex[1]].v[2]);

                glNormal3f(normals[faces[i].normal[2]].v[0], normals[faces[i].normal[2]].v[1], normals[faces[i].normal[2]].v[2]);
				glTexCoord2f(texcoords[faces[i].texture[2]].v[0],texcoords[faces[i].texture[2]].v[1]);
                glVertex3f(vertices[faces[i].vertex[2]].v[0], vertices[faces[i].vertex[2]].v[1], vertices[faces[i].vertex[2]].v[2]);
                glEnd();
            } else { // without normals -- evaluate normal on triangle

				glMaterialfv(GL_LIGHT0, GL_AMBIENT, materials[faces[i].material]->amb);
				glMaterialfv(GL_LIGHT0, GL_DIFFUSE, materials[faces[i].material]->diff);
				glMaterialfv(GL_LIGHT0, GL_SPECULAR, materials[faces[i].material]->spec);

				glBindTexture( GL_TEXTURE_2D, materials[faces[i].material]->texture.getTextureID() );

                vertex v = (vertices[faces[i].vertex[1]] - vertices[faces[i].vertex[0]]).cross(vertices[faces[i].vertex[2]] - vertices[faces[i].vertex[0]]);
                v.normalize();
                glBegin(GL_TRIANGLES);
                glNormal3f(v.v[0], v.v[1], v.v[2]);
				glTexCoord2f(texcoords[faces[i].texture[0]].v[0],texcoords[faces[i].texture[0]].v[1]);
                glVertex3f(vertices[faces[i].vertex[0]].v[0], vertices[faces[i].vertex[0]].v[1], vertices[faces[i].vertex[0]].v[2]);

				glTexCoord2f(texcoords[faces[i].texture[1]].v[0],texcoords[faces[i].texture[1]].v[1]);
                glVertex3f(vertices[faces[i].vertex[1]].v[0], vertices[faces[i].vertex[1]].v[1], vertices[faces[i].vertex[1]].v[2]);

				glTexCoord2f(texcoords[faces[i].texture[2]].v[0],texcoords[faces[i].texture[2]].v[1]);
                glVertex3f(vertices[faces[i].vertex[2]].v[0], vertices[faces[i].vertex[2]].v[1], vertices[faces[i].vertex[2]].v[2]);
                glEnd();
            }
        } else if (faces[i].vertex.size() == 4) { // quad
            if (faces[i].normal.size() == 4) { // with normals
                glBegin(GL_QUADS);
                glNormal3f(normals[faces[i].normal[0]].v[0], normals[faces[i].normal[0]].v[1], normals[faces[i].normal[0]].v[2]);
                glVertex3f(vertices[faces[i].vertex[0]].v[0], vertices[faces[i].vertex[0]].v[1], vertices[faces[i].vertex[0]].v[2]);
                glNormal3f(normals[faces[i].normal[1]].v[0], normals[faces[i].normal[1]].v[1], normals[faces[i].normal[1]].v[2]);
                glVertex3f(vertices[faces[i].vertex[1]].v[0], vertices[faces[i].vertex[1]].v[1], vertices[faces[i].vertex[1]].v[2]);
                glNormal3f(normals[faces[i].normal[2]].v[0], normals[faces[i].normal[2]].v[1], normals[faces[i].normal[2]].v[2]);
                glVertex3f(vertices[faces[i].vertex[2]].v[0], vertices[faces[i].vertex[2]].v[1], vertices[faces[i].vertex[2]].v[2]);
                glNormal3f(normals[faces[i].normal[3]].v[0], normals[faces[i].normal[3]].v[1], normals[faces[i].normal[3]].v[2]);
                glVertex3f(vertices[faces[i].vertex[3]].v[0], vertices[faces[i].vertex[3]].v[1], vertices[faces[i].vertex[3]].v[2]);
                glEnd();
            } else { // without normals -- evaluate normal on quad
                vertex v = (vertices[faces[i].vertex[1]] - vertices[faces[i].vertex[0]]).cross(vertices[faces[i].vertex[2]] - vertices[faces[i].vertex[0]]);
                v.normalize();
                glBegin(GL_QUADS);
                glNormal3f(v.v[0], v.v[1], v.v[2]);
                glVertex3f(vertices[faces[i].vertex[0]].v[0], vertices[faces[i].vertex[0]].v[1], vertices[faces[i].vertex[0]].v[2]);
                glVertex3f(vertices[faces[i].vertex[1]].v[0], vertices[faces[i].vertex[1]].v[1], vertices[faces[i].vertex[1]].v[2]);
                glVertex3f(vertices[faces[i].vertex[2]].v[0], vertices[faces[i].vertex[2]].v[1], vertices[faces[i].vertex[2]].v[2]);
                glVertex3f(vertices[faces[i].vertex[3]].v[0], vertices[faces[i].vertex[3]].v[1], vertices[faces[i].vertex[3]].v[2]);
                glEnd();
            }
        }
    }
    glEndList();
}

void cObj::render() {
    glCallList(list);
}
 
cObj::~cObj() {
    glDeleteLists(list, 1);
}