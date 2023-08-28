#include "../include/OpenGLRenderer.h"
#include <QDir>
#include <qevent.h>
#include <QApplication>

OpenGLRenderer::~OpenGLRenderer()
{
    glDisableVertexAttribArray(0);
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

void OpenGLRenderer::initializeGL() {
    timer.start();
    initializeOpenGLFunctions();
    initializeShaders();
    setupVboAndVao();
    m_last_mouse_pos = QPoint(width() / 2.0f, height() / 2.0f);
}

void OpenGLRenderer::paintGL() {
    qreal elapsedTimeSeconds = static_cast<qreal>(timer.elapsed()) / 1000.0;
    deltaTime = elapsedTimeSeconds - lastFrame;
    lastFrame = elapsedTimeSeconds;
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.85f, 0.85f, 0.85f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Use the shader program
    m_shader_program->bind();
    m_projection.setToIdentity();
    m_projection.perspective(camera->getZoom(), static_cast<float>(width() / height()), 0.1f, 100.0f);
    m_view.setToIdentity();
    m_view = camera->getViewMatrix();
    m_model.setToIdentity();
     // Bind the VAO before drawing
    m_shader_program->setUniformValue("viewMatrix", m_view);
    m_shader_program->setUniformValue("projectionMatrix", m_projection);
    m_shader_program->setUniformValue("modelMatrix", m_model);
    glBindVertexArray(m_vao);
    // Draw the triangle
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // Unbind VAO after drawing
    glBindVertexArray(0);

    m_shader_program->release();
}

void OpenGLRenderer::initializeShaders() {
    QString current_source_filepath = QString::fromUtf8(__FILE__); // Full path to the current source file
    QDir source_file_dir = QFileInfo(current_source_filepath).dir(); // Directory of the source file
    QDir shaders_dir(source_file_dir.absoluteFilePath("../shaders")); // Path to shaders directory

    bool compile_result;
    m_vertex_shader   = new QOpenGLShader(QOpenGLShader::Vertex, this);
    compile_result    = m_vertex_shader->compileSourceFile(shaders_dir.filePath("main_vert.glsl")); // Path to vertex shader file
    m_fragment_shader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    compile_result    = m_fragment_shader->compileSourceFile(shaders_dir.filePath("main_frag.glsl")); // Path to fragment shader file

    // Create and link shader program
    m_shader_program = new QOpenGLShaderProgram(this);
    m_shader_program->addShader(m_vertex_shader);
    m_shader_program->addShader(m_fragment_shader);
    m_shader_program->link(); 
}

void OpenGLRenderer::setupVboAndVao() {
    GLfloat vertices[] = {
          -0.5f, -0.5f, -0.5f,  
           0.5f, -0.5f, -0.5f, 
           0.5f,  0.5f, -0.5f, 
           0.5f,  0.5f, -0.5f,  
          -0.5f,  0.5f, -0.5f, 
          -0.5f, -0.5f, -0.5f,  

          -0.5f, -0.5f,  0.5f,
           0.5f, -0.5f,  0.5f, 
           0.5f,  0.5f,  0.5f,  
           0.5f,  0.5f,  0.5f,  
          -0.5f,  0.5f,  0.5f,  
          -0.5f, -0.5f,  0.5f,  

          -0.5f,  0.5f,  0.5f,  
          -0.5f,  0.5f, -0.5f,
          -0.5f, -0.5f, -0.5f, 
          -0.5f, -0.5f, -0.5f,  
          -0.5f, -0.5f,  0.5f, 
          -0.5f,  0.5f,  0.5f, 

           0.5f,  0.5f,  0.5f, 
           0.5f,  0.5f, -0.5f,  
           0.5f, -0.5f, -0.5f, 
           0.5f, -0.5f, -0.5f, 
           0.5f, -0.5f,  0.5f,  
           0.5f,  0.5f,  0.5f, 

          -0.5f, -0.5f, -0.5f,  
           0.5f, -0.5f, -0.5f, 
           0.5f, -0.5f,  0.5f, 
           0.5f, -0.5f,  0.5f,  
          -0.5f, -0.5f,  0.5f, 
          -0.5f, -0.5f, -0.5f, 

          -0.5f,  0.5f, -0.5f, 
           0.5f,  0.5f, -0.5f,  
           0.5f,  0.5f,  0.5f, 
           0.5f,  0.5f,  0.5f,  
          -0.5f,  0.5f,  0.5f, 
          -0.5f,  0.5f, -0.5f, 
    };

    // Create vertex buffer
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(m_vao);
    // Set up vertex attribute pointer
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);
}

void OpenGLRenderer::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
    case Qt::Key_Escape:
        QApplication::quit();
    case Qt::Key_W:
        camera->processKeyboard(CameraMovement::FORWARD, deltaTime);
        break;
    case Qt::Key_S:
        camera->processKeyboard(CameraMovement::BACKWARD, deltaTime);
        break;
    case Qt::Key_A:
        camera->processKeyboard(CameraMovement::LEFT, deltaTime);
        break;
    case Qt::Key_D:
        camera->processKeyboard(CameraMovement::RIGHT, deltaTime);
        break;
    case Qt::Key_R:
        camera->reset();
        break;
    }
    update();
}

void OpenGLRenderer::mousePressEvent(QMouseEvent* event)
{

}

void OpenGLRenderer::mouseReleaseEvent(QMouseEvent* event)
{
    mFirstTimeHandled = true;
    update();
}


void OpenGLRenderer::mouseMoveEvent(QMouseEvent* event) {
    if (event->buttons() & Qt::LeftButton) {
        if (mFirstTimeHandled)
        {
            mFirstTimeHandled = false;
            m_last_mouse_pos.setX(event->globalX());
            m_last_mouse_pos.setY(event->globalY());
        }

        float xoffset = event->globalX() - m_last_mouse_pos.x();
        float yoffset =  m_last_mouse_pos.y() - event->globalY();
        m_last_mouse_pos.setX(event->globalX());
        m_last_mouse_pos.setY(event->globalY());
        camera->processMouseMovement(xoffset, yoffset);
        update();
        //QOpenGLWidget::mouseMoveEvent(event);
   }
}

void OpenGLRenderer::wheelEvent(QWheelEvent* event) {
    int delta = event->delta();
    camera->processMouseScroll(delta);
    update();
    //event->accept();
}