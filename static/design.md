Features

### Markdown

For markdown, we used a library called cpp-markdown. It creates an object that can
read in markdown and output html.

35.161.228.19:8080/static/README.md

### gzip

We used boost/iostreams library in order to implement gzip compression. If the request to a static file contains an Accept-Encoding: header which contains gzip as a supported encoding, we send the body of the request to the CompressionHandler, with Concent-Encoding: and Content-Length: set correctly, which changes the response body to the gzipped format.

You can check the file below:

35.161.228.19:8080/static/index.html

And it will be in gzip format. If you use curl -H "Accept-Encoding: gzip", you will get gibberish, but chrome for example will uncompress the html.

You can also check our unit tests contained in CompressionHandler_test.cc, which decompresses test input and checks that they are correct.