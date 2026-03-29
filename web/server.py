from http.server import HTTPServer, SimpleHTTPRequestHandler

class MyHandler(SimpleHTTPRequestHandler):
    def end_headers(self):
        # Diese beiden Header schalten SharedArrayBuffer im Browser frei
        self.send_header("Cross-Origin-Opener-Policy", "same-origin")
        self.send_header("Cross-Origin-Embedder-Policy", "require-corp")
        super().end_headers()

if __name__ == "__main__":
    port = 8000
    print(f"Server läuft auf http://localhost:{port}")
    httpd = HTTPServer(("localhost", port), MyHandler)
    httpd.serve_forever()
