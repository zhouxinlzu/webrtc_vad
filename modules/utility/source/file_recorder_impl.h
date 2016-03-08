/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

// This file contains a class that can write audio and/or video to file in
// multiple file formats. The unencoded input data is written to file in the
// encoded format specified.

#ifndef WEBRTC_MODULES_UTILITY_SOURCE_FILE_RECORDER_IMPL_H_
#define WEBRTC_MODULES_UTILITY_SOURCE_FILE_RECORDER_IMPL_H_

#include <list>

#include "webrtc/base/platform_thread.h"
#include "webrtc/common_audio/resampler/include/resampler.h"
#include "webrtc/common_types.h"
#include "webrtc/engine_configurations.h"
#include "webrtc/modules/include/module_common_types.h"
#include "webrtc/modules/media_file/media_file.h"
#include "webrtc/modules/media_file/media_file_defines.h"
#include "webrtc/modules/utility/include/file_recorder.h"
#include "webrtc/modules/utility/source/coder.h"
#include "webrtc/system_wrappers/include/event_wrapper.h"
#include "webrtc/system_wrappers/include/tick_util.h"
#include "webrtc/typedefs.h"

namespace webrtc {
// The largest decoded frame size in samples (60ms with 32kHz sample rate).
enum { MAX_AUDIO_BUFFER_IN_SAMPLES = 60*32};
enum { MAX_AUDIO_BUFFER_IN_BYTES = MAX_AUDIO_BUFFER_IN_SAMPLES*2};
enum { kMaxAudioBufferQueueLength = 100 };

class CriticalSectionWrapper;

class FileRecorderImpl : public FileRecorder
{
public:
    FileRecorderImpl(uint32_t instanceID, FileFormats fileFormat);
    virtual ~FileRecorderImpl();

    // FileRecorder functions.
    virtual int32_t RegisterModuleFileCallback(FileCallback* callback);
    virtual FileFormats RecordingFileFormat() const;
    virtual int32_t StartRecordingAudioFile(
        const char* fileName,
        const CodecInst& codecInst,
        uint32_t notificationTimeMs) override;
    virtual int32_t StartRecordingAudioFile(
        OutStream& destStream,
        const CodecInst& codecInst,
        uint32_t notificationTimeMs) override;
    virtual int32_t StopRecording();
    virtual bool IsRecording() const;
    virtual int32_t codec_info(CodecInst& codecInst) const;
    virtual int32_t RecordAudioToFile(
        const AudioFrame& frame,
        const TickTime* playoutTS = NULL);
    virtual int32_t StartRecordingVideoFile(
        const char* fileName,
        const CodecInst& audioCodecInst,
        const VideoCodec& videoCodecInst,
        bool videoOnly = false) override
    {
        return -1;
    }
    virtual int32_t RecordVideoToFile(const VideoFrame& videoFrame) {
        return -1;
    }

protected:
    int32_t WriteEncodedAudioData(const int8_t* audioBuffer,
                                  size_t bufferLength);

    int32_t SetUpAudioEncoder();

    uint32_t _instanceID;
    FileFormats _fileFormat;
    MediaFile* _moduleFile;

private:
    CodecInst codec_info_;
    int8_t _audioBuffer[MAX_AUDIO_BUFFER_IN_BYTES];
    AudioCoder _audioEncoder;
    Resampler _audioResampler;
};
}  // namespace webrtc
#endif // WEBRTC_MODULES_UTILITY_SOURCE_FILE_RECORDER_IMPL_H_
