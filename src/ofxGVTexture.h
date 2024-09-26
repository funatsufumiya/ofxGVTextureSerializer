#pragma once

class ofxGVTexture {
protected:
    ofTexture tex;
    GLuint _texture = 0;

public:
    ofxGVTexture(const ofTexture &tex, GLuint _texture) : tex(tex), _texture(_texture) {}

    ~ofxGVTexture() {
        if (_texture != 0) {
            glDeleteTextures(1, &_texture);
        }
    }

    const ofTexture& getTexture() const {
        return tex;
    }

    void draw(float x, float y) {
        tex.draw(x, y);
    }

    void draw(float x, float y, float w, float h) {
        tex.draw(x, y, w, h);
    }
}


