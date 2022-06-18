
#include <memory>
#include "AudioEngine.h"


AudioEngine::AudioEngine() {
    createCallback();
}

AudioEngine::~AudioEngine() {

}

void AudioEngine::restart() {
    stop();
    start();
}

oboe::Result AudioEngine::createPlaybackStream() {
    oboe::AudioStreamBuilder builder;
    return builder.setSharingMode(oboe::SharingMode::Exclusive)
            ->setPerformanceMode(oboe::PerformanceMode::LowLatency)
            ->setFormat(oboe::AudioFormat::Float)
            ->setDataCallback(mDataCallback.get())
            ->setErrorCallback(mErrorCallback.get())
            ->openStream(mStream);
}

void AudioEngine::createCallback(){

    mDataCallback = std::make_unique<DefaultDataCallback>();

    // Create the error callback, we supply ourselves as the parent so that we can restart the stream
    // when it's disconnected
    mErrorCallback = std::make_unique<DefaultErrorCallback>(*this);
}

bool AudioEngine::start(){
    auto result = createPlaybackStream();

    if (result == Result::OK){
        // Create our synthesizer audio source using the properties of the stream
        mSynth = Synth::create(mStream->getSampleRate(), mStream->getChannelCount(), mNumSignals);
        mDataCallback->reset();
        mDataCallback->setSource(std::dynamic_pointer_cast<IRenderableAudio>(mSynth));
        mStream->start();
        return true;
    } else {
        LOGE("Failed to create the playback stream. Error: %s", convertToText(result));
        return false;
    }
}

bool AudioEngine::stop() {
    if(mStream && mStream->getState() != oboe::StreamState::Closed) {
        mStream->stop();
        mStream->close();
    }
    mStream.reset();
    return true;
}