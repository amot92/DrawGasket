// Two-Dimensional Sierpinski Gasket
// Generated using randomly selected vertices and bisection


#include "GL/glew.h"
#include <stdio.h>
#include <stdlib.h>
#include "GLFW/glfw3.h"

const int NumPoints = 5000;
GLuint shader_programme;
GLuint vao;
GLuint buffer;
int I = 0;
int cnt = 0;
GLfloat points[(NumPoints*2)*10];
//----------------------------------------------------------------------------
void init( void )
{
    
    GLuint vert_shader, frag_shader;
    /* GL shader programme object [combined, to link] */
    
    // Create and initialize a buffer object
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );
    
    // Create a vertex array object
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // "attribute #0 is created from every 2 variables in the above buffer, of type
    // float (i.e. make me vec2s)"
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    
    // Load shaders and use the resulting shader program
    const char *vertex_shader = "#version 410\n"
    "in vec4 vPosition;"
    "void main () {"
    "    gl_Position = vPosition;"
    "}";
    /* the fragment shader colours each fragment (pixel-sized area of the
     triangle) */
    const char *fragment_shader = "#version 410\n"
    "out vec4 FragColor;"
    "void main () {"
    "    FragColor = vec4( 1.0, 0.0, 0.0, 1.0 );"
    "}";
    
    vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vertex_shader, NULL);
    glCompileShader(vert_shader);
    
    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &fragment_shader, NULL);
    glCompileShader(frag_shader);
    
    shader_programme = glCreateProgram();
    glAttachShader(shader_programme, frag_shader);
    glAttachShader(shader_programme, vert_shader);
    glLinkProgram(shader_programme);
}

void drawGasket(float x, float y)
{
    GLfloat vertices[] = {x - 0.25f, y -0.25f,  x + 0.0f, y+0.25f,  x+0.25f, y-0.25f };
    points[I] = x + 0.0625;
    points[I+1] = y - 0.125;
    for (int i = 2; i < NumPoints*2; i+=2) {
        int j = rand()%3;   // pick a vertex at random
        // Compute the point halfway between the selected vertex
        //  and the previous point
        points[I+i] = (points[I+i - 2] + vertices[2*j]) / 2.0;
        points[I+i+1] = (points[I+i - 1] + vertices[2 * j+1]) / 2.0;
    }
    cnt += NumPoints;
    I += NumPoints*2;
    
}

void mymouse(GLFWwindow* window, int button, int action, int mods){
    if (GLFW_PRESS == action && button == GLFW_MOUSE_BUTTON_LEFT) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        x = (2.0f * x / 640 )- 1.0f;
        y = 1.0f - (2.0f * y) / 640;
        drawGasket(x, y);//add new points to points[] array
        //glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
        //glGenVertexArrays(1, &vao);

        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        // "attribute #0 is created from every 2 variables in the above buffer, of type
        // float (i.e. make me vec2s)"
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    } else if (GLFW_PRESS == action && button == GLFW_MOUSE_BUTTON_RIGHT){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

//----------------------------------------------------------------------------

int main( int argc, char **argv )
{
    GLFWwindow *window = NULL;
    const GLubyte *renderer;
    const GLubyte *version;
    /* start GL context and O/S window using the GLFW helper library */
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window = glfwCreateWindow(640, 480, "Sierpinski Gasket", NULL, NULL);
    
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    /* start GLEW extension handler */
    glewExperimental = GL_TRUE;
    glewInit();
    renderer = glGetString(GL_RENDERER); /* get renderer string */
    version = glGetString(GL_VERSION);     /* version as a string */
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);
    init();
    
     glfwSetMouseButtonCallback(window, mymouse);
    
    do{
        glClear(GL_COLOR_BUFFER_BIT);     // clear the window
        glUseProgram(shader_programme);
        glBindVertexArray(vao);
        glDrawArrays(GL_POINTS, 0, cnt);    // draw the points
        /* update other events like input handling */
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    while (!glfwWindowShouldClose(window));
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
    
}


