#include "lab_m1/Tema2/Tema2.h"


#include <vector>
#include <iostream>

#include "core/engine.h"
#include "utils/gl_utils.h"

#include "lab_m1/Tema2/Transform3D.h"

using namespace std;
using namespace m1;

int ma_map[14][14] = {
    {1, 1, 1, 1, 1, 3, 1, 1, 1, 3, 1, 3, 3, 1},
    {0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1},
    {0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0},
    {1, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1},
    {1, 0, 2, 0, 3, 3, 3, 3, 3, 1, 3, 3, 3, 1},
    {1, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1},
    {3, 1, 3, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 1},
    {3, 3, 3, 3, 0, 0, 0, 3, 3, 3, 3, 3, 3, 1},
    {3, 3, 3, 3, 0, 2, 0, 0, 3, 1, 3, 3, 3, 0},
    {1, 3, 3, 0, 0, 0, 0, 2, 0, 1, 1, 0, 0, 0},
    {3, 3, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 3, 3, 0, 0, 3, 0, 0, 0, 2, 0, 0, 0, 1},
    {1, 3, 3, 3, 3, 3, 3, 3, 1, 0, 0, 1, 1, 3},
    {1, 1, 1, 1, 3, 1, 1, 3, 3, 3, 3, 3, 3, 1},
};

int inamici[10][2];
int k = 0;

float unghi = 0;

int verificare = 0;
int constanta = 1;

float damagedLength = 0;
float maxLength = 0.5;
int passedTime = 0;

int viteza = 0;

int shoot = -1;
int currentX = 0;
int currentY = 0;
int currentZ = 0;
int vitezaProiectil = 0;

bool third = true;

Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{

    translateX = 0;
    translateY = 0;
    translateZ = 0;

    end = 0;

    renderCameraTarget = false;
    
    for (int i = 0; i < 14; i++) {
        srand(time(0));
        for (int j = 0; j < 14; j++) {
            if (ma_map[i][j] == 3) {
               
                int x = rand();
                if (x % 2 == 0) {
                    ma_map[i][j] = 0;
                }
                else {
                    ma_map[i][j] = 1;
                }
            }
            if (ma_map[i][j] == 4) {

                int x = rand() % 3;
                if (x == 0) {
                    ma_map[i][j] = 0;
                }
                else if (x == 1) {
                    ma_map[i][j] = 1;
                }
                else if (x == 2) {
                    ma_map[i][j] = 2;
                }
            }
        }
    }

    camera = new implemented_2::Camera();
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    // Create a simple cube
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(-1, -1,  1), glm::vec3(0, 1, 1), glm::vec3(0.2, 0.8, 0.2)),
            VertexFormat(glm::vec3(1, -1,  1), glm::vec3(1, 0, 1), glm::vec3(0.9, 0.4, 0.2)),
            VertexFormat(glm::vec3(-1,  1,  1), glm::vec3(1, 0, 0), glm::vec3(0.7, 0.7, 0.1)),
            VertexFormat(glm::vec3(1,  1,  1), glm::vec3(0, 1, 0), glm::vec3(0.7, 0.3, 0.7)),
            VertexFormat(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1), glm::vec3(0.3, 0.5, 0.4)),
            VertexFormat(glm::vec3(1, -1, -1), glm::vec3(0, 1, 1), glm::vec3(0.5, 0.2, 0.9)),
            VertexFormat(glm::vec3(-1,  1, -1), glm::vec3(1, 1, 0), glm::vec3(0.7, 0.0, 0.7)),
            VertexFormat(glm::vec3(1,  1, -1), glm::vec3(0, 0, 1), glm::vec3(0.1, 0.5, 0.8)),
        };

        vector<unsigned int> indices =
        {
            0, 1, 2,        1, 3, 2,
            2, 3, 7,        2, 7, 6,
            1, 7, 3,        1, 5, 7,
            6, 7, 4,        7, 5, 4,
            0, 4, 1,        1, 4, 5,
            2, 6, 4,        0, 2, 4,
        };

        CreateMesh("cube", vertices, indices);
    }

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader* shader = new Shader("TemaShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader2 = new Shader("TemaShader2");
        shader2->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader2.glsl"), GL_VERTEX_SHADER);
        shader2->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader2.glsl"), GL_FRAGMENT_SHADER);
        shader2->CreateAndLink();
        shaders[shader2->GetName()] = shader2;
    }

    {
        Shader* shader3 = new Shader("TemaShaderEnemy");
        shader3->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShaderEnemy.glsl"), GL_VERTEX_SHADER);
        shader3->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShaderEnemy.glsl"), GL_FRAGMENT_SHADER);
        shader3->CreateAndLink();
        shaders[shader3->GetName()] = shader3;
    }

    {
        Shader* shader4 = new Shader("TemaShaderLife");
        shader4->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShaderLife.glsl"), GL_VERTEX_SHADER);
        shader4->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShaderLife.glsl"), GL_FRAGMENT_SHADER);
        shader4->CreateAndLink();
        shaders[shader4->GetName()] = shader4;
    }

    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
}



Mesh* Tema2::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    meshes[name]->vertices = vertices;
    meshes[name]->indices = indices;
    return meshes[name];
}




void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glFlush();

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}



void Tema2::Update(float deltaTimeSeconds)
{
    window->DisablePointer();
    if (end == 0) {
        renderCameraTarget = third;
        
        if (renderCameraTarget) {
            Player();
        }
       
        Map();
        for (int i = 0; i < k; i++) {
            Inamic(inamici[i][0], inamici[i][1]);
        }

        Life();
        Time();
        if (!renderCameraTarget) {
            Proiectil();
        }
   
        if (renderCameraTarget)
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
            RenderSimpleMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
        }
    }
    if (end == 1) {
        cout << endl << " END GAME !!! " << endl;
        exit(0);
    }
}


void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    glUseProgram(shader->program);

    int modelLocation = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &modelMatrix[0][0]);

   // glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    int viewLocation = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &viewMatrix[0][0]);


    //glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int projLocation = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void Tema2::Proiectil() {

    glm::mat4 ProiectilModelMatrix = glm::mat4(1);
    vitezaProiectil = vitezaProiectil + 1;
    if(shoot == 1){
        
        ProiectilModelMatrix = glm::translate(ProiectilModelMatrix, glm::vec3(currentX, currentY, currentZ - vitezaProiectil * 0.1));
        ProiectilModelMatrix = glm::translate(ProiectilModelMatrix, glm::vec3(0, 0.28f, 0));
        ProiectilModelMatrix = glm::rotate(ProiectilModelMatrix, unghi, glm::vec3(0, 1, 0));
        ProiectilModelMatrix = glm::scale(ProiectilModelMatrix, glm::vec3(0.04f));
        RenderSimpleMesh(meshes["cube"], shaders["TemaShader"], ProiectilModelMatrix);
    }

   

}


void Tema2::Life()  //bara verde
{
    glm::mat4 LifeModelMatrix = glm::mat4(1);
    LifeModelMatrix = glm::translate(LifeModelMatrix, glm::vec3(camera->GetTargetPosition()));
    LifeModelMatrix = glm::rotate(LifeModelMatrix, unghi, glm::vec3(0, 1, 0));
    glm::vec3 corner1 = { -1.1, 0.75, 0.65 };
    
    Mesh* dreptunghi1 = CreateRectangle("dreptunghi1", corner1, 0.1, 0.5, glm::vec3(0, 1, 0), false);
    AddMeshToList(dreptunghi1);
    RenderSimpleMesh(meshes["dreptunghi1"], shaders["TemaShaderLife"], LifeModelMatrix);

    float alive = 0.5;

    if (damagedLength >= 0.5) {
        alive = 0;
    }

    if (damagedLength >= 0.5) {

        end = 1;
    }

    Mesh* dreptunghi2 = CreateRectangle("dreptunghi2", corner1, 0.1, alive, glm::vec3(0, 1, 0), true);
    AddMeshToList(dreptunghi2);
    RenderSimpleMesh(meshes["dreptunghi2"], shaders["TemaShaderLife"], LifeModelMatrix);
}

void Tema2::Time()   // bara rosie
{
    float currentLength;
    passedTime++;

    glm::mat4 TimeModelMatrix = glm::mat4(1);
    TimeModelMatrix = glm::translate(TimeModelMatrix, glm::vec3(camera->GetTargetPosition()));
    TimeModelMatrix = glm::rotate(TimeModelMatrix, unghi, glm::vec3(0, 1, 0));
    glm::vec3 corner2 = { -1.1, 0.6, 0.65 };

    Mesh* dreptunghi3 = CreateRectangle("dreptunghi3", corner2, 0.1, 0.5, glm::vec3(0, 1, 0), false);
    AddMeshToList(dreptunghi3);
    RenderSimpleMesh(meshes["dreptunghi3"], shaders["TemaShaderEnemy"], TimeModelMatrix);

    if (passedTime % 200 == 0) {

        damagedLength += 0.05;
    }
    
    currentLength = maxLength - damagedLength;

    if (currentLength < 0) {
        currentLength = 0;
    }

    Mesh* dreptunghi4 = CreateRectangle("dreptunghi4", corner2, 0.1, currentLength, glm::vec3(0, 1, 0), true);
    AddMeshToList(dreptunghi4);
    RenderSimpleMesh(meshes["dreptunghi4"], shaders["TemaShaderEnemy"], TimeModelMatrix);
}

Mesh* Tema2::CreateRectangle(const std::string& name, glm::vec3 leftBottomCorner, float height, float width, glm::vec3 color, bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(width, 0, 0), color),
        VertexFormat(corner + glm::vec3(width, height, 0), color),
        VertexFormat(corner + glm::vec3(0, height, 0), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}

void Tema2::Map()
{
   
   
    for (int i = 0; i < 14; i = i + 1)
    {
        
            for (int j = 0; j < 14; j = j + 1)
            {
                
                if(ma_map[i][j] == 1)
                {
                    glm::mat4 newmodelMatrix = glm::mat4(1);
                    float a = 2 * i - 2;
                    float b = -6 - 2 * j;
                    newmodelMatrix = glm::translate(newmodelMatrix, glm::vec3(a, 0, b));
                    newmodelMatrix = glm::scale(newmodelMatrix, glm::vec3(1));
                    RenderSimpleMesh(meshes["cube"], shaders["TemaShader2"], newmodelMatrix);
                }

                if (ma_map[i][j] == 2)
                {
                    if (verificare == 0) {
                        inamici[k][0] = i;
                        inamici[k][1] = j;
                        k++;
                    }
                    
                }

                if (i == j == 13) {
                    verificare = 1;
                }
               
            }
    }


}

void Tema2::Inamic(int i, int j)
{
    glm::mat4 modelMatrixEnemy = glm::mat4(1);
    float a = 2 * i - 2;
    float b = -6 - 2 * j;
    
    
    viteza++;

    modelMatrixEnemy = glm::translate(modelMatrixEnemy, glm::vec3(a, 0, b));
    modelMatrixEnemy = glm::scale(modelMatrixEnemy, glm::vec3(0.2, 0.5, 0.2));

    if (viteza % 1000 == 0) {
       
        constanta = constanta * (-1);  //schimba directia
        viteza = 0;
    }

    modelMatrixEnemy *= Transform3D::Translate(a + 0.0005 * constanta * viteza, 0, b - 0.0005 * constanta * viteza);
    
    RenderSimpleMesh(meshes["cube"], shaders["TemaShaderEnemy"], modelMatrixEnemy);
}

void Tema2::Player()
{
    //17 cuburi player

    //brat stang
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z));
        modelMatrix = glm::rotate(modelMatrix, unghi, glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.180f, 0.16f, 0));
        
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.04f));

        
        RenderSimpleMesh(meshes["cube"], shaders["TemaShader"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z));
        modelMatrix = glm::rotate(modelMatrix, unghi, glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.180f, 0.28f, 0));
       
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.04f));

        
        RenderSimpleMesh(meshes["cube"], shaders["TemaShader"], modelMatrix);
    }
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z));
        modelMatrix = glm::rotate(modelMatrix, unghi, glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.180f, 0.36f, 0));
      
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.04f));

       
        RenderSimpleMesh(meshes["cube"], shaders["TemaShader"], modelMatrix);
    }

    //brat drept
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z));
        modelMatrix = glm::rotate(modelMatrix, unghi, glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.180f, 0.16f, 0));
       
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.04f));

       
        RenderSimpleMesh(meshes["cube"], shaders["TemaShader"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z));
        modelMatrix = glm::rotate(modelMatrix, unghi, glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.180f, 0.28f, 0));
       
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.04f));

       
        RenderSimpleMesh(meshes["cube"], shaders["TemaShader"], modelMatrix);
    }
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z));
        modelMatrix = glm::rotate(modelMatrix, unghi, glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.180f, 0.36f, 0));
        
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.04f));

       
        RenderSimpleMesh(meshes["cube"], shaders["TemaShader"], modelMatrix);
    }

    //corp-centru

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z));
        modelMatrix = glm::rotate(modelMatrix, unghi, glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.2f, 0));
        
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.04f));

       
        RenderSimpleMesh(meshes["cube"], shaders["TemaShader"], modelMatrix);
    }
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z));
        modelMatrix = glm::rotate(modelMatrix, unghi, glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.28f, 0));
        
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.04f));

       
        RenderSimpleMesh(meshes["cube"], shaders["TemaShader"], modelMatrix);
    }
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z));
        modelMatrix = glm::rotate(modelMatrix, unghi, glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.36f, 0));
        
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.04f));

       
        RenderSimpleMesh(meshes["cube"], shaders["TemaShader"], modelMatrix);
    }

    //corp-stanga

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z));
        modelMatrix = glm::rotate(modelMatrix, unghi, glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.08f, 0.20f, 0));
       
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.04f));

      
        RenderSimpleMesh(meshes["cube"], shaders["TemaShader"], modelMatrix);
    }
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z));
        modelMatrix = glm::rotate(modelMatrix, unghi, glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.08f, 0.28f, 0));
        
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.04f));

       
        RenderSimpleMesh(meshes["cube"], shaders["TemaShader"], modelMatrix);
    }
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z));
        modelMatrix = glm::rotate(modelMatrix, unghi, glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.08f, 0.36f, 0));
       
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.04f));

      
        RenderSimpleMesh(meshes["cube"], shaders["TemaShader"], modelMatrix);
    }

    //corp-dreapta

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z));
        modelMatrix = glm::rotate(modelMatrix, unghi, glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.08f, 0.20f, 0));
        
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.04f));

      
        RenderSimpleMesh(meshes["cube"], shaders["TemaShader"], modelMatrix);
    }
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z));
        modelMatrix = glm::rotate(modelMatrix, unghi, glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.08f, 0.28f, 0));
        
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.04f));

      
        RenderSimpleMesh(meshes["cube"], shaders["TemaShader"], modelMatrix);
    }
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z));
        modelMatrix = glm::rotate(modelMatrix, unghi, glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.08f, 0.36f, 0));
       
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.04f));

    
        RenderSimpleMesh(meshes["cube"], shaders["TemaShader"], modelMatrix);
    }

    // cap
    {
        glm::mat4 modelMatrix = glm::mat4(1);

        modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z));
        modelMatrix = glm::rotate(modelMatrix, unghi, glm::vec3(0, 1, 0));

        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.460f, 0));


        
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.04f));

       
       
        RenderSimpleMesh(meshes["cube"], shaders["TemaShader"], modelMatrix);
    }

    //picior stang

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z));
        modelMatrix = glm::rotate(modelMatrix, unghi, glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.08f, 0, 0));
        
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.04f));

     
        RenderSimpleMesh(meshes["cube"], shaders["TemaShader"], modelMatrix);
    }
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z));
        modelMatrix = glm::rotate(modelMatrix, unghi, glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.08f, 0.08f, 0));
      
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.04f));

      
        RenderSimpleMesh(meshes["cube"], shaders["TemaShader"], modelMatrix);
    }

    //picior drept
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z));
        modelMatrix = glm::rotate(modelMatrix, unghi, glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.08f, 0, 0));
       
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.04f));

        
        RenderSimpleMesh(meshes["cube"], shaders["TemaShader"], modelMatrix);
    }
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z));
        modelMatrix = glm::rotate(modelMatrix, unghi, glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.08f, 0.08f, 0));

        
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.04f));

       
        RenderSimpleMesh(meshes["cube"], shaders["TemaShader"], modelMatrix);
    }
}


void Tema2::FrameEnd()
{
    //DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}





void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    float speed = 100.f;
    float rotationSpeed = 2.f;
    float cameraSpeed = 2.f;

    if (window->KeyHold(GLFW_KEY_W)) {
        translateZ -= deltaTime * speed;
        camera->MoveForward(cameraSpeed * deltaTime);
    }
    else if (window->KeyHold(GLFW_KEY_S)) {
        translateZ += deltaTime * speed;
        camera->MoveForward(-cameraSpeed * deltaTime);
       
       
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        translateX -= deltaTime * speed;
        camera->TranslateRight(-cameraSpeed * deltaTime);
    }
    else if (window->KeyHold(GLFW_KEY_D)) {
        translateX += deltaTime * speed;
        camera->TranslateRight(cameraSpeed * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_Q)) {
        translateY += deltaTime * speed;
        camera->TranslateUpward(-cameraSpeed * deltaTime);
    }
    else if (window->KeyHold(GLFW_KEY_E)) {
        translateY -= deltaTime * speed;
       camera->TranslateUpward(cameraSpeed * deltaTime);
    }


    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float cameraSpeed = 2.f;

    }
    

    if (window->KeyHold(GLFW_KEY_MINUS)) {

        projectionMatrix = glm::perspective(-RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

    }

    if (window->KeyHold(GLFW_KEY_EQUAL)) {

        projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

    }

    if (window->KeyHold(GLFW_KEY_0)) {

        float offsetX = 1, offsetY = 1;
        projectionMatrix = glm::ortho(-50.f - offsetX, 50.f + offsetX, -50.f - offsetY, 50.f + offsetY, 0.01f, 1000.f);
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_SPACE) {
        shoot = shoot * (-1);
        if (shoot == 1) {
            vitezaProiectil = 0;
            currentX = camera->GetTargetPosition().x;
            currentY = camera->GetTargetPosition().y;
            currentZ = camera->GetTargetPosition().z;
        }
   
    }
    
    
    if (key == GLFW_KEY_LEFT_CONTROL) {
        third = !third;
        if (third == true)
            camera->TranslateForward(-camera->distanceToTarget);
        else
            camera->TranslateForward(+camera->distanceToTarget);
    }

    if (key == GLFW_KEY_T)
    {
        renderCameraTarget = !renderCameraTarget;
    }

    if (key == GLFW_KEY_P)
    {
        projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
    }

    if (key == GLFW_KEY_O)
    {
        projectionMatrix = glm::ortho(-50.f, 50.f, -50.f, 50.f, 0.01f, 1000.f);
    }
}


void Tema2::OnKeyRelease(int key, int mods)
{

}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
   // if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (third == false) {
            
            
            unghi += -deltaX * sensivityOX;

            camera->RotateThirdPerson_OX(-deltaY * sensivityOY);
            camera->RotateThirdPerson_OY(-deltaX * sensivityOX);
        }

        else {
            
            

            unghi += -deltaX * sensivityOX;

            camera->RotateFirstPerson_OX(-deltaY * sensivityOY);
            camera->RotateFirstPerson_OY(-deltaX * sensivityOX);
        }

       
    }

}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}





