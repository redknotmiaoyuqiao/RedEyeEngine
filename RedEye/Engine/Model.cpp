#include "Engine.hpp"
#include "RedLog/Debug.hpp"
#include <string>

Model::Model(std::string path)
{
    meshs = new std::vector<Mesh*>();

    int index = 0;
    while(true){
        std::string file_path = path + "/part" + intToString(index);

        FILE * fb = fopen(file_path.c_str(), "rb");

        if(fb == NULL){
            break;
        }

        int numVertices, numFaces;

        fread(&numVertices, sizeof(unsigned int), 1, fb);
        fread(&numFaces, sizeof(unsigned int), 1, fb);

		RedLog("\nLoad Mesh Form:\n%s\n\n", file_path.c_str());
        RedLog("\nnumVertices:%d\n",numVertices);
        RedLog("\nnumFaces:%d\n",numFaces);

        float * vertexArray = (float *)malloc(sizeof(float) * numVertices * 3);
        float * textureCoordArray = (float *)malloc(sizeof(float) * numVertices * 3);
        float * normalArray = (float *)malloc(sizeof(float) * numVertices * 3);
        float * tangentsArray = (float *)malloc(sizeof(float) * numVertices * 3);

        unsigned int * indicesArray = (unsigned int *)malloc(sizeof(unsigned int) * numFaces * 3);

        fread(textureCoordArray,sizeof(float) * 3,numVertices,fb);
        fread(vertexArray,sizeof(float) * 3,numVertices,fb);
        fread(normalArray,sizeof(float) * 3,numVertices,fb);
        fread(tangentsArray,sizeof(float) * 3,numVertices,fb);
        fread(indicesArray,sizeof(unsigned int),numFaces * 3,fb);

        fclose(fb);


        std::vector<Vertex*> * vertices = new std::vector<Vertex*>();
        std::vector<GLuint> * indices = new std::vector<GLuint>();

        for(int i=0;i<numVertices;i++){
            Vertex * v = new Vertex;
            v->Position_x = vertexArray[i * 3];
            v->Position_y = vertexArray[i * 3 + 1];
            v->Position_z = vertexArray[i * 3 + 2];

            v->Normal_x = normalArray[i * 3];
            v->Normal_y = normalArray[i * 3 + 1];
            v->Normal_z = normalArray[i * 3 + 2];

            v->Tangents_x = tangentsArray[i * 3];
            v->Tangents_y = tangentsArray[i * 3 + 1];
            v->Tangents_z = tangentsArray[i * 3 + 2];

            v->TexCoords_x = textureCoordArray[i * 3];
            v->TexCoords_y = textureCoordArray[i * 3 + 1];
            vertices->push_back(v);
        }

        for(int i=0;i<numFaces * 3;i++){
            indices->push_back(indicesArray[i]);
        }

        free(vertexArray);
        free(textureCoordArray);
        free(normalArray);
        free(tangentsArray);
        free(indicesArray);


		std::string name = "part" + intToString(index);
        Mesh * mesh = new Mesh(vertices,indices, name);
        meshs->push_back(mesh);

        index++;
    }
}

Model::~Model()
{
   for (std::vector<Mesh*>::iterator it = meshs->begin(); it != meshs->end(); it++){
       delete *it;
   }
   meshs->clear();
   delete meshs;
}

void Model::DrawAllVAO()
{
    for (std::vector<Mesh*>::iterator it = meshs->begin(); it != meshs->end(); it++){
        (*it)->vao->DrawVAO();
    }
}

/*
void Model::Draw(GLProgram * program)
{
   for (std::vector<Mesh*>::iterator it = meshs->begin(); it != meshs->end(); it++){
       (*it)->Draw(program);
   }
}
*/
