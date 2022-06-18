#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include <oboe/Oboe.h>

using namespace oboe;

class AudioEngine : public IRestartable {

public:
    AudioEngine();
    virtual ~AudioEngine();

private:
    std::shared_ptr<AudioStream> mStream;
    std::shared_ptr<Synth> mSynth;
    std::unique_ptr<DefaultDataCallback> mDataCallback;
    std::unique_ptr<DefaultErrorCallback> mErrorCallback;

    oboe::Result createPlaybackStream();
    void createCallback();

};


#endif // AUDIO_ENGINE_H