//ISCRTAVA TROUGAO
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = R"s(
#version 330 core
layout (location = 0) in vec3 aPos; //definisemo sta svaki od atributa znaci, na poziciji 0 pocinje atribut koji je vektor
//dimenzije 3 i predstavlja koordinate vertexa
void main()
{
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); //gl_Position je promenljiva koja cuva poziciju vertexa
}

)s";

const char *fragmentShaderSource = R"s(
#version 330 core
out vec4 FragColor;

unifor vec4 Color;
void main()
{
   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); //izlazna promenljiva dimenzije 4 koja definise boju svakog fragmenta koje trougao prekriva
}
)s";

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER); //pravimo novi shader, vraca dimenziju shadera
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //prosledjujemo vertexu kod sa vertexShaderSourca
    glCompileShader(vertexShader);//kompiliramo shader
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); //proverava da li je sve uspelo 
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
   //sve isto kao za vertex
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    int shaderProgram = glCreateProgram(); //pravimo shader program na koji kacimo shadere
    glAttachShader(shaderProgram, vertexShader);//kacimo vertex
    glAttachShader(shaderProgram, fragmentShader);//kacimo fragment
    glLinkProgram(shaderProgram);//linkujemo
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);//brisemo shadere, ne trebaju nam vise jer smo ih linkovali 

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left  
         0.5f, -0.5f, 0.0f, // right 
         0.0f,  0.5f, 0.0f  // top   
    }; //temena trougla (cuva atribute vertexa)

    unsigned int VBO, VAO; 
    glGenVertexArrays(1, &VAO); //pravi 1(prvi argument) vertex array objekat i njegov identifikator upisuje u adresu VAO objekta
   //prvi argument postoji jer nam je nekad potrebno dosta vertex array objekata i efikasnije je da ih samo napravimo odjednom
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO); //sve dalje funkcije koje rade bilo sta sa VAO ce se odnositi na ovaj specifican VAO objekat

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //alocira memoriju na GPU ako vec nije alocirana
   //4. argument je hint openglu za sta koristimo ovu memoriju (static - ne menja se cesto, draw - crta)
   //hint sluzi za bolju implementaciju u OpenGlu 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//0-broj atributa, 3-broj vrednosti u atributu(pozicije),
   //tip atributa, da li zelimo da se normalizuje, velicina vertexa u memoriji kao broj bajtova, 0 jer u svakom vertexu taj atribut pocinje
   //na 0. bajtu
    glEnableVertexAttribArray(0); //kazemo da atribut 0(poziciju) koristimo u iscrtavanju

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); //deaktivira objekat 1. argumenta u narednim iscrtavanjima(0-drugi arg)

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //cisti bafer na ovu boju
        glClear(GL_COLOR_BUFFER_BIT); //postavlja tu boju 
       
       int colorLocation = glGetUniformLocation(shaderProgram, "Color"); //vraca lokaciju uniforme prom 
       glUniform4f(colorLocation, 0.5f, 0.2f, 0.1f, 0,4f);

        // draw our first triangle
       //iscrtavanje
        glUseProgram(shaderProgram);//koji program koristimo za iscrtavanje, sharedProgram je lokalna promenljiva
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3); //koristi sve aktivirane programe za iscrtavanje, pokrece pipeline i salje sve na crtanje
        // glBindVertexArray(0); // no need to unbind it every time 
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);//tek ovde se vide promene
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
