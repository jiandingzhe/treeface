#ifndef TREEJUCE_NATIVE_LINUX_PRIVATE_H
#define TREEJUCE_NATIVE_LINUX_PRIVATE_H

#include "treejuce/InputStream.h"
#include "treejuce/StringRef.h"
#include "treejuce/URL.h"

TREEFACE_JUCE_NAMESPACE_BEGIN

class WebInputStream  : public InputStream
{
public:
    WebInputStream (const String& address_, bool isPost_, const MemoryBlock& postData_,
                    URL::OpenStreamProgressCallback* progressCallback, void* progressCallbackContext,
                    const String& headers_, int timeOutMs_, StringPairArray* responseHeaders);

    ~WebInputStream();

    //==============================================================================
    bool isError() const                 { return socketHandle < 0; }
    bool isExhausted() override          { return finished; }
    int64 getPosition() override         { return position; }

    int64 getTotalLength() override
    {
        //xxx to do
        return -1;
    }

    int read (void* buffer, int bytesToRead) override;

    bool setPosition (int64 wantedPos) override;

    //==============================================================================
    int statusCode;

private:
    int socketHandle, levelsOfRedirection;
    StringArray headerLines;
    String address, headers;
    MemoryBlock postData;
    int64 position;
    bool finished;
    const bool isPost;
    const int timeOutMs;

    void closeSocket();

    int createConnection (URL::OpenStreamProgressCallback* progressCallback, void* progressCallbackContext);

    //==============================================================================
    String readResponse (const int socketHandle, const uint32 timeOutTime);

    static void writeValueIfNotPresent (MemoryOutputStream& dest, const String& headers, const String& key, const String& value)
    {
        if (! headers.containsIgnoreCase (key))
            dest << "\r\n" << key << ' ' << value;
    }

    static void writeHost (MemoryOutputStream& dest, const bool isPost, const String& path, const String& host, const int port)
    {
        dest << (isPost ? "POST " : "GET ") << path << " HTTP/1.0\r\nHost: " << host;
    }

    static MemoryBlock createRequestHeader (const String& hostName, const int hostPort,
                                            const String& proxyName, const int proxyPort,
                                            const String& hostPath, const String& originalURL,
                                            const String& userHeaders, const MemoryBlock& postData,
                                            const bool isPost);

    static bool sendHeader (int socketHandle, const MemoryBlock& requestHeader, const uint32 timeOutTime,
                            URL::OpenStreamProgressCallback* progressCallback, void* progressCallbackContext);

    static bool decomposeURL (const String& url, String& host, String& path, int& port);

    static String findHeaderItem (const StringArray& lines, const String& itemName);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WebInputStream)
};

TREEFACE_JUCE_NAMESPACE_END

#endif // TREEJUCE_NATIVE_LINUX_PRIVATE_H
