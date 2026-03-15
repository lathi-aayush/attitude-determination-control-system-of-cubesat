# Future Scope – AarambhSat ADCS Exhibition Upgrade

Planned upgrades for project exhibition competition. Decisions and design notes are captured below.

---

## 1. Bluetooth

- **Module:** HC-05 (classic Bluetooth, serial profile).
- **Connectivity (decided):** **Option 1** — A second HC-05 (or Bluetooth dongle with SPP) on the PC/laptop, paired with the HC-05 on the Arduino. The backend running on the PC opens the Bluetooth serial port and talks to the Arduino wirelessly. No USB cable between PC and CubeSat during exhibition.

---

## 2. Command Language

- **Syntax:** Underscore preferred (e.g. `right_90`).
- **Commands:**
  - `right_90`, `right_180` — rotate right by 90° or 180°.
  - `left_90`, `left_180` — rotate left by 90° or 180°.
  - `hold` — maintain current orientation (target = current angle).
  - `hold N` — hold for N seconds, then continue to next command in sequence.
  - `stop` — stop immediately (emergency / reset).
  - `auto` — switch to LDR-based mode (sun/light tracking); CubeSat uses LDR sensors for orientation.
- **Sequences:** Comma-separated list, e.g. `right_90, hold 2, left_90`.
- **Sequence execution (decided):** **Sequence is stored and run on the Arduino.** The website sends the full sequence string once; the Arduino parses it, queues the steps, and executes them. The Arduino can echo back current step and status so the website can display “which command is being executed” and “what is the sequence.”

---

## 3. Web Interface (Exhibition Front)

- **Concept:** Only the website is visible at the exhibition. Arduino and all other hardware are hidden. A PC/laptop runs a local backend that bridges the browser to the Arduino (over Bluetooth serial).
- **Architecture:** **Website (browser)** ↔ **Backend (PC)** ↔ **Bluetooth serial** ↔ **HC-05** ↔ **Arduino.**

**Website to display (design to be provided later):**

- **Connect Bluetooth** — button/status: Connecting / Connected / Disconnected / Error (backend connects to Bluetooth serial port).
- **Current position** — e.g. angle (from Arduino telemetry).
- **System status** — e.g. “All systems active” / “Running properly” (from Arduino status messages).
- **Mode** — Auto (LDR) vs Command (manual/sequence).
- **Command input** — field for single command or sequence (e.g. `right_90, hold 2, left_90`).
- **Current sequence** — list of steps being run (from Arduino or echoed at start).
- **Currently executing** — e.g. “Step 2: hold 2” or “right_90”.
- Other details as needed (e.g. Send, Clear, Stop).

**Note:** Browsers cannot access Bluetooth/serial directly; the backend on the PC is required to open the port and relay between web UI and Arduino.

---

## 4. Arduino Telemetry (for the website)

Arduino sends periodic or event-driven lines over serial so the backend can forward to the website, e.g.:

- **Position** — e.g. `angle: 45` or `yaw: 90`.
- **Mode** — `mode: auto` or `mode: command`.
- **System status** — e.g. `ok` / `error` / wheel saturation or fault codes.
- **Current command / step** — e.g. `cmd: hold` or `step: 2, cmd: hold 2`.
- **Sequence (optional)** — list of steps when a new sequence is loaded, so the website can show “what is the sequence.”

Format can be simple text lines (one key-value or short sentence per line) for easy parsing.

---

## 5. Optional Exhibition Enhancements (later)

- **LED feedback** — e.g. “listening”, “executing”, “error”.
- **Safety limits** — max angle per command, timeout so a bad command doesn’t spin indefinitely.
- **Demo mode** — e.g. a single command that runs a fixed loop (e.g. right_90 → hold 2 → left_90) for unattended demos.
- **Stop** — always available on the website, maps to `stop` on the Arduino.

---

## Summary of Decided Items

| Item | Decision |
|------|----------|
| Bluetooth module | HC-05 |
| PC ↔ Arduino link | Option 1: Second HC-05 (or BT dongle) on PC, paired with Arduino’s HC-05; backend uses Bluetooth serial. |
| Where sequence runs | Sequence stored and run on Arduino; full sequence sent once from website. |
| Command syntax | Underscore preferred (e.g. `right_90`). |
| Roll command | Omitted. |
| Auto mode | Added; LDR-based orientation when in `auto` mode. |

Design for the website layout and visuals will be provided separately; this document defines scope and technical decisions.
