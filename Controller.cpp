#include "Controller.h"
#include "sgraph/IScenegraph.h"
#include "sgraph/Scenegraph.h"
#include "sgraph/GroupNode.h"
#include "sgraph/LeafNode.h"
#include "sgraph/ScaleTransform.h"
#include "ObjImporter.h"
using namespace sgraph;
#include <iostream>
using namespace std;

#include "sgraph/ScenegraphExporter.h"
#include "sgraph/ScenegraphImporter.h"

Controller::Controller(Model& m,View& v) {
    model = m;
    view = v;
    filePathExists = false;
    initScenegraph();
}

Controller::Controller(Model& m,View& v, char* fp) {
    model = m;
    view = v;
    filePath = fp;
    filePathExists = true;
    initScenegraph();
}

void Controller::initScenegraph() {
    if(filePathExists){
        ifstream inFile(filePath);
        //ifstream inFile("tryout.txt");
        sgraph::ScenegraphImporter importer;
        
    
        IScenegraph *scenegraph = importer.parse(inFile);
        //scenegraph->setMeshes(meshes);
        model.setScenegraph(scenegraph);
        cout <<"Scenegraph made" << endl;   
    }
    else {
        ifstream inFile("scenegraphmodels/castle.txt");
        //ifstream inFile("tryout.txt");
        sgraph::ScenegraphImporter importer;
        
    
        IScenegraph *scenegraph = importer.parse(inFile);
        //scenegraph->setMeshes(meshes);
        model.setScenegraph(scenegraph);
        cout <<"Scenegraph made" << endl;   
    }

}

Controller::~Controller()
{
    
}

void Controller::run()
{
    sgraph::IScenegraph * scenegraph = model.getScenegraph();
    map<string,util::PolygonMesh<VertexAttrib> > meshes = scenegraph->getMeshes();
    view.init(this,meshes);
    while (!view.shouldWindowClose()) {
        view.display(scenegraph);
        if(!released) {
            if (count == 0) {
                view.findMousePos(true);
                count++;
            }
            //view.rotate();
            view.findMousePos(false);
        }
        else {
            count = 0;
            // view.dontRotate();
        }
    }
    view.closeWindow();
    exit(EXIT_SUCCESS);
}

void Controller::onkey(int key, int scancode, int action, int mods)
{
    cout << (char)key << " pressed" << endl;
    if (key == GLFW_KEY_R){ // 'r' or 'R'
        //reset trackball

    }
}

void Controller::onMouse(int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        cout << "left mouse being pressed" << endl;
        released = false;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        cout << "left mouse being released" << endl;
        released = true;
    }
}

void Controller::reshape(int width, int height) 
{
    cout <<"Window reshaped to width=" << width << " and height=" << height << endl;
    glViewport(0, 0, width, height);
}

void Controller::dispose()
{
    view.closeWindow();
}

void Controller::error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}