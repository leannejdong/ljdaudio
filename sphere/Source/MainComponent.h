#pragma once

#include <JuceHeader.h>
using namespace juce::gl;

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::OpenGLAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void initialise() override;
    void shutdown() override;
    void render() override;
    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
    std::vector<float> sphereVertices;
    std::vector<unsigned int> sphereIndices;
    GLuint vboHandle = 0; // Vertex Buffer Object handle
    GLuint iboHandle = 0; // Index Buffer Object handle
    GLuint vaoHandle = 0; // Vertex Array Object handle

    GLuint shaderProgram = 0;


    const char* vertexShader = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;

        void main()
        {
            gl_Position = vec4(aPos, 1.0);
        }
    )";

    const char* fragmentShader = R"(
        #version 330 core
        out vec4 FragColor;

        void main()
        {
            FragColor = vec4(0.5f, 0.0f, 0.5f, 1.0f); //Purple color
        }
    )";


    std::pair<std::vector<float>, std::vector<unsigned int>> generateSphereVertices(float radius, unsigned int rings, unsigned int sectors);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
