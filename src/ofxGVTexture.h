#pragma once

class ofxGVTexture {
protected:
    ofTexture tex;
    GLuint _texture = 0;
    bool _isAllocated = false;

public:
    ofxGVTexture(const ofTexture& tex, GLuint _texture) : tex(tex), _texture(_texture) {
        _isAllocated = true;
    }

    ofxGVTexture() {
    }

    ~ofxGVTexture() {
        if (_texture != 0) {
            glDeleteTextures(1, &_texture);
        }
    }

    const ofTexture& getTexture() const {
        return tex;
    }

    bool isAllocated() const {
        return _isAllocated;
    }

    void draw(float x, float y) {
        tex.draw(x, y);
    }

    void draw(float x, float y, float w, float h) {
        tex.draw(x, y, w, h);
    }
};
