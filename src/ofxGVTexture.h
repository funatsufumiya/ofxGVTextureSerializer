#pragma once

#include "ofMain.h"
#include <memory>

class ofxGVTexture {
protected:
    // std::shared_ptr<ofTexture> tex;
    ofTexture tex;
    GLuint _texture = 9999;

public:
    // ofxGVTexture(std::shared_ptr<ofTexture> tex, GLuint _texture) : tex(tex), _texture(_texture) {
    ofxGVTexture(ofTexture tex, GLuint _texture) : tex(tex), _texture(_texture) {
    }

    ofxGVTexture() {
    }

    // delete copy constructor to suppress invalid deletion of texture
    ofxGVTexture(const ofxGVTexture&) = delete;

    // move constructor
    ofxGVTexture(ofxGVTexture&& other) : tex(std::move(other.tex)), _texture(other._texture) {
        other._texture = 9999;
    }

    // delete copy assignment operator to suppress invalid deletion of texture
    ofxGVTexture& operator=(const ofxGVTexture&) = delete;

    // move assignment operator
    ofxGVTexture& operator=(ofxGVTexture&& other) {
        if (this != &other) {
            tex = std::move(other.tex);
            _texture = other._texture;
            other._texture = 0;
        }
        return *this;
    }

    ~ofxGVTexture() {
        if (_texture != -1) {
            // int refCount = tex.use_count();
            // if (refCount <= 1) {
            //     glDeleteTextures(1, &_texture);
            // }
            glDeleteTextures(1, &_texture);
        }
    }

    const ofTexture& getTexture() const {
        return tex;
    }

    bool isAllocated() const {
        return tex.isAllocated();
    }

    void draw(float x, float y) {
        tex.draw(x, y);
    }

    void draw(float x, float y, float w, float h) {
        tex.draw(x, y, w, h);
    }
};
