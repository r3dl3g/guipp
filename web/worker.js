// worker.js
importScripts('@PROJECT_NAME@.wasm.js');

// 1. Global deklarieren
self.canvas = null;
self.rect = null;
self.eventQueue = [];

self.onmessage = function(e) {
    if (e.data.type === 'init') {
        self.canvas = e.data.canvas;
        self.rect = e.data.rect;

        // Emscripten Modul starten
        createModule({
            // Wir können das Canvas auch hier nochmal sicherheitshalber injizieren
            preRun: [function(mod) {
                mod.canvas = e.data.canvas;
                mod.rect = e.data.rect;
            }]
        }).then(instance => {
            const eventData = {
                type:   6,
                state:  0,
                x:      e.data.rect.width,
                y:      e.data.rect.height,
                key:    ""
            };
            self.eventQueue.push(eventData);
            console.log("Worker: Wasm Instanz läuft.");
        });
    } else {
        // 2. Events in das Array pushen
        // Wir stellen sicher, dass x und y immer existieren (Standard 0)
        const eventData = {
            type:   e.data.type,
            state:  e.data.state !== undefined ? e.data.state : 0,
            x:      e.data.x !== undefined ? e.data.x : 0,
            y:      e.data.y !== undefined ? e.data.y : 0,
            key:    e.data.key || ""
        };

        self.eventQueue.push(eventData);
        // Optionaler Log zur Kontrolle in der Browser-Konsole
        // console.log("Event in Queue eingereiht:", eventData.type);
    }
};
