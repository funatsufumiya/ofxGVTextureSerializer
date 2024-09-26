#pragma once

#include "ofMain.h"
#include <memory>

class ofxGVTexture {
protected:
    std::shared_ptr<ofTexture> tex;
    GLuint _texture = 0;

public:
    ofxGVTexture(std::shared_ptr<ofTexture> tex, GLuint _texture) : tex(tex), _texture(_texture) {
    }

    ofxGVTexture() {
    }

    // copy constructor to suppress

    ~ofxGVTexture() {
        if (_texture != 0) {
            int refCount = tex.use_count();
            if (refCount <= 1) {
                glDeleteTextures(1, &_texture);
            }
        }
    }

    const ofTexture& getTexture() const {
        return *tex.get();
    }

    bool isAllocated() const {
        return tex->isAllocated();
    }

    void draw(float x, float y) {
        tex->draw(x, y);
    }

    void draw(float x, float y, float w, float h) {
        tex->draw(x, y, w, h);
    }
};
