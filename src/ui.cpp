#include "GE/Core/Debug/log.hpp"
#include "GE/Ressources/ui.hpp"
#include "GE/Core/System/UISystem.hpp"


using namespace Engine::Ressources;
using namespace Engine::Core::Maths;
using namespace Engine::Core::Debug;
using namespace Engine::Core::Systems;


Button::Button(float _x, float _y, float _w, float _h, Vec3 _color)
    : shader("./ressources/shader/2d.vert", "./ressources/shader/color.frag")
{
    x = _x;
    y = _y;
    w = _w;
    h = _h;
    this->color = _color;


    float wid = 1400;
    float hei = 900;

    float tempx1 = ((_x) / wid * 2) - 1;
    float tempy1 = 1 - ((_y) / hei * 2);
    float tempx2 = ((_x + _w) / wid * 2) - 1;
    float tempy2 = 1 - ((_y + _h) / hei * 2);

    GLfloat vertices[] = {tempx1, tempy1, 0.0f,
                          tempx2, tempy1, 0.0f,
                          tempx2, tempy2, 0.0f,
                          tempx1, tempy2, 0.0f};

    GLuint indices[] = {0, 1, 2, 0, 2, 3};

    GLuint VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    UISystem::addButton(this);  
}

void Button::draw()
{
    glDisable(GL_DEPTH_TEST);
    
    Mat4 matrix;
    GLfloat tempcolor[3] = {this->color.x, this->color.y, this->color.z};

    GLuint matrixID = glGetUniformLocation(shader.getIdProgramm(), "matrix");
    GLuint colorID = glGetUniformLocation(shader.getIdProgramm(), "color");
    shader.use();
    glUniformMatrix4fv(matrixID, 1, GL_TRUE, &matrix[0][0]);
    glUniform3fv(colorID, 1, &tempcolor[0]);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
    
    glEnable(GL_DEPTH_TEST);
}

bool Button::isButtonPressed(float x, float y, int state)
{
    if (state)
    {
        if (x >= this->x && x <= this->x + this->w
        &&  y >= this->y && y <= this->y + this->h)
        {
            this->function();
            this->color = Vec3{0.5f, 0.5f, 0.5f};
            return true;
        }
    }
    this->color = Vec3{1.f, 1.f, 1.f};
    return false;
}

Button::~Button()
{
    glDeleteVertexArrays(1, &VAO);
    UISystem::removeButton(this);
}

TextField::TextField(const char* TTFPath, float _x, float _y, float _w, float _h, const std::string& _string)
    : shader("./ressources/shader/text.vert", "./ressources/shader/texture.frag")
{
    color.r = 255;
    color.g = 255;
    color.b = 255;
    x = _x;
    y = _y;
    w = _w;
    h = _h;
    
    value = _string;

    std::string ttfpathstring = TTFPath;

    font = TTF_OpenFont(TTFPath, 18);
    if (!font)
        SLog::logError("failed to open : " + ttfpathstring);

    updateTexture();

    UISystem::addTextField(this);
}

TextField::~TextField()
{   
 	TTF_CloseFont(font);
    glDeleteVertexArrays(1, &VAO);
    UISystem::removeTextField(this);
}

void TextField::updateTexture()
{
    glDeleteBuffers(1, &VAO);
/////////////////////////

    int bpp;
    Uint32 Rmask, Gmask, Bmask, Amask;
    SDL_PixelFormatEnumToMasks(SDL_PIXELFORMAT_ABGR8888, &bpp, &Rmask, &Gmask, &Bmask, &Amask);
    SDL_Surface *img_rgba8888 = SDL_CreateRGBSurface(0, w, h, bpp, Rmask, Gmask, Bmask, Amask);
    SDL_Surface* surface = TTF_RenderText_Solid(font, value.c_str(), color);
    SDL_BlitSurface(surface, NULL, img_rgba8888, NULL);
    SDL_FreeSurface(surface);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_rgba8888->pixels);
    SDL_FreeSurface(img_rgba8888);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, texture);

/////////////////////////////

    float wid = 1400;
    float hei = 900;

    float tempx1 = ((x) / wid * 2) - 1;
    float tempy1 = 1 - ((y) / hei * 2);
    float tempx2 = ((x + w) / wid * 2) - 1;
    float tempy2 = 1 - ((y + h) / hei * 2);

    GLfloat vertices[] = {tempx1, tempy1, 0.0f,
                          tempx2, tempy1, 0.0f,
                          tempx2, tempy2, 0.0f,
                          tempx1, tempy2, 0.0f};

    GLfloat uvs[]      = {0.0f, 0.0f, 0.0f,
                          1.0f, 0.0f, 0.0f,
                          1.0f, 1.0f, 0.0f,
                          0.0f, 1.0f, 0.0f};

    GLuint indices[] = {0, 1, 2, 0, 2, 3};

    GLuint VBO, VBO2, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VBO2);
    glDeleteBuffers(1, &EBO);  
}

void TextField::draw()
{
    glDisable(GL_DEPTH_TEST);
    shader.use();
    glBindTexture(GL_TEXTURE_2D, texture);
    
    Mat4 matrix = Mat4::createTRSMatrix({0,0,0},{0,0,0},{1,1,1});
    GLuint matrixID = glGetUniformLocation(shader.getIdProgramm(), "matrix");

    glUniformMatrix4fv(matrixID, 1, GL_TRUE, &matrix[0][0]);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);    
    glEnable(GL_DEPTH_TEST);
}

bool TextField::isTextFieldactive(float x, float y)
{
    if (x >= this->x && x <= this->x + this->w
    &&  y >= this->y && y <= this->y + this->h
    && !active)
    {
        active = true;
        wasActive = true;
        color.r = 255;
        color.g = 0;
        color.b = 0;
        
        updateTexture();
        return true;
    }
    else
        desactivateTextField();
    
    return false;
}

void TextField::desactivateTextField()
{   
    color.r = 255;
    color.g = 255;
    color.b = 255;
    active = false;
    if (wasActive)
    {
        updateTexture();
        wasActive = false;
    }
}

void TextField::updateString(char toAdd)
{
    if (!active)
        return;

    color.r = 255;
    color.g = 0;
    color.b = 0;

    value += toAdd;
    function();
    updateTexture();
}
void TextField::stringDel()
{
    if (value.size() == 0)
        return;
    value.resize(value.size() - 1);
    if (value.size() >= 1)
        function();
    updateTexture();
}

UIBox::UIBox()
{
    
}

UIBox::~UIBox()
{

}

