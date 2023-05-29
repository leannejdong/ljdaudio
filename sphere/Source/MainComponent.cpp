#include "MainComponent.h"
//using namespace juce::gl;

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);
}

MainComponent::~MainComponent()
{
    // This shuts down the GL system and stops the rendering calls.
    shutdownOpenGL();
}

//==============================================================================
void MainComponent::initialise()
{
    // // Initialise GL objects for rendering here.
    // sphereVertices = generateSphereVertices(1.0f, 30, 30);

    auto [vertices, indices] = generateSphereVertices(1.0f, 30, 30);
    sphereVertices = vertices;
    sphereIndices = indices;
    // Create VBO
    glGenBuffers(1, &vboHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), sphereVertices.data(), GL_STATIC_DRAW);
    // // Create VAO
    // glGenVertexArrays(1, &vaoHandle);
    // glBindVertexArray(vaoHandle);

    // // Enable attribute 0 in VAO and link it to VBO
    // glEnableVertexAttribArray(0);
    // glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Create VAO
    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);

    // Enable attribute 0 in VAO and link it to VBO
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Create and compile vertex shader
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vertexShader, NULL);
    glCompileShader(vertShader);

    // Create and compile fragment shader
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragmentShader, NULL);
    glCompileShader(fragShader);

    // Link shaders into a shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);

    // Clean up shaders (we don't need them anymore after they've been linked into the program)
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);


}

void MainComponent::shutdown()
{
    // Free any GL objects created for rendering here.
     glDeleteBuffers(1, &vboHandle);
    glDeleteBuffers(1, &iboHandle);
    glDeleteVertexArrays(1, &vaoHandle);
    glDeleteProgram(shaderProgram);
}

void MainComponent::render()
{
    // This clears the context with a black background.
    juce::OpenGLHelpers::clear (juce::Colours::transparentBlack);

    // Add your rendering code here...
    juce::gl::glEnable(juce::gl::GL_DEPTH_TEST);
    juce::gl::glDepthFunc(juce::gl::GL_LESS);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, getWidth(), getHeight());

        // Use the shader program
    glUseProgram(shaderProgram);

    // Bind the VAO and issue a draw call
    glBindVertexArray(vaoHandle);
    glDrawArrays(GL_POINTS, 0, sphereVertices.size() / 3);//this gives a single dot

    // Unbind the VAO
    glBindVertexArray(0);

}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // You can add your component specific drawing code here!
    // This will draw over the top of the openGL background.
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}

std::pair<std::vector<float>, std::vector<unsigned int>> MainComponent::generateSphereVertices(float radius, unsigned int rings, unsigned int sectors)
{
    // std::vector<float> vertices;
    // vertices.reserve(rings * sectors * 6);

    // const float R = 1.0f / (float)(rings - 1);
    // const float S = 1.0f / (float)(sectors - 1);

    // for(unsigned int r = 0; r < rings; r++)
    // {
    //     for(unsigned int s = 0; s < sectors; s++)
    //     {
    //         const float y = sin(-M_PI_2 + M_PI * r * R);
    //         const float x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
    //         const float z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

    //         vertices.push_back(x * radius);
    //         vertices.push_back(y * radius);
    //         vertices.push_back(z * radius);
    //     }
    // }

    // return vertices;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    vertices.reserve(rings * sectors * 3);
    indices.reserve(rings * sectors * 4);

    const float R = 1.0f / (float)(rings - 1);
    const float S = 1.0f / (float)(sectors - 1);

    for(unsigned int r = 0; r < rings; r++)
    {
        for(unsigned int s = 0; s < sectors; s++)
        {
            const float y = sin(-M_PI_2 + M_PI * r * R);
            const float x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
            const float z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

            vertices.push_back(x * radius);
            vertices.push_back(y * radius);
            vertices.push_back(z * radius);
            
            indices.push_back(r * sectors + s);
            indices.push_back(r * sectors + (s + 1));
            indices.push_back((r + 1) * sectors + (s + 1));
            indices.push_back((r + 1) * sectors + s);
        }
    }

    return std::make_pair(vertices, indices);
}
