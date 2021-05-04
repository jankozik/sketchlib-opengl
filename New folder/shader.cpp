#include "shader.h"

void Shader::LoadShader(GLenum type, const std::string& filepath) {
    GLuint shader_id = glCreateShader(type);
    std::string shader_code;

    std::ifstream file_stream(filepath, std::ios::in);
    if (file_stream.is_open()) {
        std::stringstream buffer;
        buffer << file_stream.rdbuf();
        shader_code = buffer.str();
        file_stream.close();
    }
    else {
        return 0;  // path does not exist, deem the shader file optional and return 0.
    }

    printf("Compiling shader file : %s\n", filepath.c_str());

    const char* shader = shader_code.c_str();
    glShaderSource(shader_id, 1, &shader, NULL);
    glCompileShader(shader_id);

    GLint status;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
        GLint info_log_length;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);

        GLchar* info_log = new GLchar[info_log_length + 1];
        glGetShaderInfoLog(shader_id, info_log_length, NULL, info_log);

        fprintf(stderr, "Failed to compile shader : %s\n", info_log);
        delete[] info_log;
        glDeleteShader(shader_id);  // prevent shader leak

        std::cin.get();  // pause the console before exiting so that we can read error messages
        exit(EXIT_FAILURE);
    }

    shaders.push_back(shader_id);
}

void Shader::LinkShaders() {
    printf("Linking shader files ...\n\n");
    GLuint program_id = glCreateProgram();

    for (auto&& shader : shaders) {
        if (shader > 0) {
            glAttachShader(program_id, shader);
        }
    }

    glLinkProgram(program_id);

    GLint status;
    glGetProgramiv(program_id, GL_LINK_STATUS, &status);

    if (status == GL_FALSE) {
        GLint info_log_length;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);

        GLchar* info_log = new GLchar[info_log_length + 1];
        glGetProgramInfoLog(program_id, info_log_length, NULL, info_log);

        fprintf(stderr, "Failed to link shaders : %s\n", info_log);
        delete[] info_log;
    }

    for (auto&& shader : shaders) {
        if (shader > 0) {
            glDetachShader(program_id, shader);
        }
    }

    id = program_id;
}

GLint Shader::GetUniformLocation(const std::string& name) const {
    if (uniform_loc_cache.find(name) != uniform_loc_cache.end()) {
        return uniform_loc_cache[name];
    }

    GLint location = glGetUniformLocation(id, name.c_str());
    if (location != -1) {
        uniform_loc_cache[name] = location;
    }
    else {
        printf("Uniform location not found : %s\n", name.c_str());
    }
    return location;
}

Shader::Shader(const std::string& filepath) {
    // load all shaders from the given path
    LoadShader(GL_VERTEX_SHADER, filepath + "vertex.glsl");
    LoadShader(GL_FRAGMENT_SHADER, filepath + "fragment.glsl");
    LoadShader(GL_GEOMETRY_SHADER, filepath + "geometry.glsl");
    LoadShader(GL_COMPUTE_SHADER, filepath + "compute.glsl");

    // link all shaders to create shader program
    LinkShaders();

    // clean up
    std::for_each(shaders.begin(), shaders.end(), glDeleteShader);
}

Shader::~Shader() {
    glDeleteProgram(id);
}

void Shader::Bind() const {
    glUseProgram(id);
}

void Shader::Unbind() const {
    glUseProgram(0);
}

void Shader::SetBool(const std::string& name, bool value) const {
    glUniform1i(GetUniformLocation(id, name), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const {
    glUniform1i(GetUniformLocation(id, name), value);
}

void Shader::SetFloat(const std::string& name, float value) const {
    glUniform1f(GetUniformLocation(id, name), value);
}

void Shader::SetVec2(const std::string& name, const glm::vec2& value) const {
    glUniform2fv(GetUniformLocation(id, name), 1, &value[0]);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(GetUniformLocation(id, name), 1, &value[0]);
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(GetUniformLocation(id, name), 1, &value[0]);
}

void Shader::SetMat2(const std::string& name, const glm::mat2& value) const {
    glUniformMatrix2fv(GetUniformLocation(id, name), 1, GL_FALSE, &value[0][0]);
}

void Shader::SetMat3(const std::string& name, const glm::mat3& value) const {
    glUniformMatrix3fv(GetUniformLocation(id, name), 1, GL_FALSE, &value[0][0]);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& value) const {
    glUniformMatrix4fv(GetUniformLocation(id, name), 1, GL_FALSE, &value[0][0]);
}
