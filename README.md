# ofxGVTextureSerializer

GVTextureSerializer addon for OpenFrameworks 0.11/0.12

You can use this for Texture sharing as bytes, via ZeroMQ / Shared Memory etc.

## Usage

- ofApp.h
    
    ```cpp
    ofxGVTextureSerializer serializer;
    ofFbo fbo; // source
    ofTexture tex; // destination
    ofBuffer buffer; // serialized data
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

        // serialize fbo to buffer
        ofBuffer buffer = serializer.serializeTexture(fbo.getTexture());

        // deserialize buffer to tex
        tex = serializer.deserialize(buffer);
    }

    void ofApp::draw() {
        ofSetColor(255);
        fbo.draw(0, 0);
        tex.draw(512, 0);
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

This format is nearly same as [ofxExtremeGpuVideo](https://github.com/Ushio/ofxExtremeGpuVideo).
