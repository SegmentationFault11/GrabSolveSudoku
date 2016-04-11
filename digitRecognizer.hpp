#ifndef digitRecognizer_hpp
#define digitRecognizer_hpp

#include <tesseract/baseapi.h>
#include <tesseract/strngs.h>

class Recognizer {
public:
    tesseract::TessBaseAPI tesseract;
    
    Recognizer();
    
    
};

#endif /* digitRecognizer_hpp */
