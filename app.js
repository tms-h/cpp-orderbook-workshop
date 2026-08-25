const slides = [...document.querySelectorAll(".slide")];
const counterCurrent = document.querySelector(".slide-counter b");
const counterTotal = document.querySelector(".slide-counter span");
const progress = document.querySelector(".progress-track span");
const actLabel = document.querySelector(".act-label");
const help = document.querySelector(".help-dialog");
const closeHelp = document.querySelector(".dialog-close");

let current = Math.max(0, Math.min(slides.length - 1, Number(location.hash.slice(1)) - 1 || 0));

function showSlide(next, updateHash = true) {
  current = Math.max(0, Math.min(slides.length - 1, next));

  slides.forEach((slide, index) => {
    slide.classList.toggle("is-active", index === current);
    slide.classList.toggle("is-before", index < current);
    slide.setAttribute("aria-hidden", index === current ? "false" : "true");
  });

  counterCurrent.textContent = String(current + 1).padStart(2, "0");
  counterTotal.textContent = `/ ${String(slides.length).padStart(2, "0")}`;
  progress.style.width = `${((current + 1) / slides.length) * 100}%`;
  actLabel.textContent = slides[current].dataset.act;
  const slideTitle = slides[current].querySelector("h1, h2")?.innerText
    .replace(/\s+/g, " ")
    .trim();
  document.title = `${slideTitle || "C++ in Quant Finance"} · ${current + 1}/${slides.length}`;

  if (updateHash) history.replaceState(null, "", `#${current + 1}`);
}

function next() { showSlide(current + 1); }
function previous() { showSlide(current - 1); }

document.querySelector(".next").addEventListener("click", next);
document.querySelector(".prev").addEventListener("click", previous);
closeHelp.addEventListener("click", () => help.close());

document.addEventListener("keydown", (event) => {
  if (help.open && event.key !== "Escape" && event.key.toLowerCase() !== "h") return;

  if (["ArrowRight", "PageDown", " "].includes(event.key)) {
    event.preventDefault();
    next();
  } else if (["ArrowLeft", "PageUp"].includes(event.key)) {
    event.preventDefault();
    previous();
  } else if (event.key === "Home") {
    showSlide(0);
  } else if (event.key === "End") {
    showSlide(slides.length - 1);
  } else if (event.key.toLowerCase() === "f") {
    document.fullscreenElement ? document.exitFullscreen() : document.documentElement.requestFullscreen();
  } else if (event.key.toLowerCase() === "h") {
    help.open ? help.close() : help.showModal();
  }
});

window.addEventListener("hashchange", () => {
  const requested = Number(location.hash.slice(1)) - 1;
  if (Number.isFinite(requested)) showSlide(requested, false);
});

function paintOrderFlow() {
  const canvas = document.querySelector(".order-painting");
  const context = canvas.getContext("2d");
  const dpr = Math.min(window.devicePixelRatio || 1, 2);
  const bounds = canvas.getBoundingClientRect();

  canvas.width = Math.max(1, Math.floor(bounds.width * dpr));
  canvas.height = Math.max(1, Math.floor(bounds.height * dpr));
  context.scale(dpr, dpr);
  context.clearRect(0, 0, bounds.width, bounds.height);

  const hand = getComputedStyle(document.documentElement).getPropertyValue("--hand");
  const prices = [103, 102, 100, 99];
  const colors = ["#bd4b43", "#bd4b43", "#2858a6", "#2858a6"];
  const lengths = [.38, .62, .76, .46];
  const startY = bounds.height * .25;
  const gap = bounds.height * .13;

  context.globalCompositeOperation = "multiply";
  context.lineCap = "round";
  context.font = `700 18px ${hand}`;

  prices.forEach((price, index) => {
    const y = startY + gap * index;
    const x = bounds.width * .27;
    context.fillStyle = colors[index];
    context.globalAlpha = .95;
    context.fillText(`$${price}`, x - 54, y + 6);

    for (let pass = 0; pass < 3; pass += 1) {
      context.beginPath();
      context.moveTo(x, y + pass * 1.5);
      context.lineTo(x + bounds.width * lengths[index], y + (pass - 1) * 2.2);
      context.strokeStyle = colors[index];
      context.globalAlpha = .4;
      context.lineWidth = 8 - pass * 2;
      context.stroke();
    }
  });

  context.globalAlpha = .8;
  context.strokeStyle = "#17202a";
  context.lineWidth = 2.5;
  context.setLineDash([7, 6]);
  context.beginPath();
  context.moveTo(bounds.width * .19, startY + gap * 1.5);
  context.lineTo(bounds.width * .9, startY + gap * 1.5 + 4);
  context.stroke();
  context.setLineDash([]);

  context.font = `700 16px ${hand}`;
  context.fillStyle = "#17202a";
  context.globalAlpha = .9;
  context.fillText("SELL", bounds.width * .74, startY - 27);
  context.fillText("BUY", bounds.width * .74, startY + gap * 3.1 + 32);

  context.strokeStyle = "#bd4b43";
  context.lineWidth = 3;
  context.beginPath();
  context.arc(bounds.width * .18, startY + gap * 1.5, 31, -.4, Math.PI * 1.72);
  context.stroke();
}

showSlide(current, false);
paintOrderFlow();
window.addEventListener("resize", paintOrderFlow);
