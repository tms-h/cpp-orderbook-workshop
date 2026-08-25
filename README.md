# C++ in Quant Finance

> **Workshop in development.** Content, presenter allocation, and checkpoint code are still being reviewed.

A beginner-friendly workshop that introduces electronic trading by progressively constructing a simple limit order book in C++.

No prior C++ or finance knowledge is required. Basic programming experience is recommended.

## Workshop shape

| Time | Act | Purpose |
| ---: | --- | --- |
| 15 min | Opening | Presenters, quant roles, why quant, why C++ |
| 10 min | Order-book overview | Limit prices, two sides, spread, crossing, price–time priority |
| 70 min | Implementation | Six progressive, tested checkpoints |
| 10 min | Reflection | Real systems, resources, quant opportunities, questions |

## Repository

- `index.html` — HTML presentation
- `styles.css` — visual system and print layout
- `app.js` — slide navigation and hand-drawn order-book artwork
- `checkpoints/` — C++ templates and completed checkpoints for copying into CoderPad
- `GOAL_PROMPT.md` — durable Codex goal prompt for continuing the project

## Run the presentation

The deck has no external runtime dependencies.

```bash
python3 -m http.server 8080
```

Then open `http://localhost:8080`.

Keyboard controls:

- `←` / `→` — navigate
- `Space` — next slide
- `Home` / `End` — first / last slide
- `F` — fullscreen
- `H` — help

## Teaching principles

1. Start from a market question, not a language feature.
2. Let the audience predict before showing the rule.
3. Make each checkpoint small, runnable, and recoverable.
4. Test market behaviour rather than a particular container choice.
5. Preserve time for price–time priority and the final matching payoff.

## References

- [Optiver — Orders and the order book](https://www.optiver.com/insights/explainers/orders-and-the-order-book/)
- [Digg — RL-Trained Coding Model Generates JavaScript Paintings](https://digg.com/tech/6btzztrd)

The presentation recreates all visuals independently. References inform the teaching sequence and possible future procedural-art experiments; no third-party visual assets are included.
