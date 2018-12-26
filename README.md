# A Truly Janky Server

I wanted to discover the wonderful world of network programming. This is the resulting mistake.

# Compile & Run

It does not run on Windows *per se*, but you can run it from WSL if you insist. For Linux users, the usual: `make && make clean`. As I'm not trying to compete with anyone else, please just run it in its own directory. You certainly *can* set it up the "right" way if you tell `make` to point the `BASEDIR` to wherever your distro usually stores web-facing files, but be advised: *Here there be quite a lot of very angry dragons*.

At any rate, PHP is a soft(-ish) dependency, in the sense that there is no default action in the event PHP is missing or otherwise unusable.

# TODO

What works:

- Serving HTML/CSS
- Serving PHP/CSS
- Serving error pages (partially implemented)

What does not work yet:

- Serving basically anything that is not text

What has not been tested:

- All of the above on an actual Linux install (it's been mostly tested on WSL)

What has not been implemented:

- Logging
- gzip response compression
- JSON/XML responses
- Routing
- Decent multithreading support
- Support for uploads
