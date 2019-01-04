╔══════════════════════════════════════════════════════════════════════════════════════╗
║                                                                                      ║
║   ████████ ████████ ██    ██ ██   ▄██ ██    ██ ▄███████ ████████ ██████▄  ██    ██   ║
║         ██ ██▀  ▀██ ███   ██ ██ ▄██▀  ▀██  ██▀ ██▀      ██       ██    ██ ██    ██   ║
║         ██ ██    ██ ████  ██ ████▀     ▀████▀  ██▄      ██       ██    ██  ██  ██    ║
║        ███ █████ ██ ██ ██ ██ ███        ▀██▀   ▀██████▄ ██████   ██████▀   ██  ██    ║
║        ███ ██    ██ ██  ████ ████▄       ██         ▀██ ██       ██▀██▄     ████     ║
║   ██  ████ ██    ██ ██   ███ ██ ▀██▄     ██         ▄██ ██       ██  ▀██▄   ████     ║
║   ████████ ██    ██ ██    ██ ██   ▀██    ██    ███████▀ ████████ ██    ██    ██      ║
║                                                                                      ║
╚══════════════════════════════════════════════════════════════════════════════════════╝

# A Truly Janky Server

I wanted to discover the wonderful world of network programming. This is the resulting mistake.

# Compile & Run

It does not run on Windows *per se*, but you can run it from WSL if you insist. For Linux users, the usual: `make && make clean`. As I'm not trying to compete with anyone else, please just run it in its own directory. You certainly *can* set it up the "right" way if you tell `make` to point the `BASEDIR` define to wherever your distribution usually stores web-facing files, but be advised: *Here there be quite a lot of very angry dragons*.

At any rate, PHP is a soft(-ish) dependency. You can make it without PHP support (`USE_PHP=0`), in which case the server will spit out a 501 error page if you ask it to work on a .php file.

# CHANGELOG: v0.12a

- Hacked POST request support into the PHP worker (seriously it's *that* ugly)
- Hardcoded redirection from / to index.php. Eventually this will be rewritten when I implement proper config-based redirection

# TODO

What works:

- Serving HTML/CSS
- Serving PHP/CSS
- Serving error pages (only 403, 404, 418, 500 and 501 are implemented)
- Redirecting (hardcoded)

What does not work yet:

- Serving basically anything that is not text (and JSON)

What has not been tested:

- Nearly anything regarding security (to be fair if you're running this in anything other than a test intranet environment, you're about to be visited by a lot of dragons)
- Simultaneous connections

What has not been implemented (more or less in order of when it should be implemented):

- Image support
- JSON responses
- gzip response compression
- Upload support
- Logging
- Unit tests
- Redirection (using a definition file instead of hardcoding)
- Decent multithreading support
