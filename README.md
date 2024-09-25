# ofxGVTextureSerializer

***WIP***

GVTextureSerializer addon for OpenFrameworks 11/12

You can use this for Texture sharing as bytes, via ZeroMQ / Shared Memory etc.

## Usage

***WIP***

- ofApp.h
    
    ```cpp
    ofxGVTextureSerializer serializer;
    ofFbo fbo;
    ofTexture tex;
    ```

- ofApp.cpp

    ```cpp
    void ofApp::setup() {
        fbo.allocate(512, 512, GL_RGBA);
    }

    void ofApp::update() {
        fbo.begin();
        ofClear(0);
        // draw rotating triangle
        ofPushMatrix();
        ofTranslate(fbo.getWidth() / 2, fbo.getHeight() / 2);
        ofRotateDeg(ofGetElapsedTimef() * 30);
        ofDrawTriangle(0, -100, 87, 100, -87, 100);
        ofPopMatrix();
        fbo.end();
    }

    void ofApp::draw() {
        ofSetColor(255);
        fbo.draw(0, 0);
        tex.draw(0, 512);
    }
    ```

## Binary File Format (GVTexture)

```txt
0: uint32_t width
4: uint32_t height
12: uint32_t format (DXT1 = 1, DXT3 = 3, DXT5 = 5, BC7 = 7)
16: uint32_t frame bytes
20: raw frame storage (lz4 compressed)
```