import { Link } from "react-router-dom";
import { useState } from "react";
import { MobileNav } from "../components/MobileNav";
import { SiteHeader } from "../components/SiteHeader";
import { IMG } from "../media";

const HARDWARE = [
  {
    title: "Arduino Uno",
    tag: "Brain",
    tagClass: "bg-primary/10 text-primary border-primary/20",
    img: IMG.arduino,
    desc: "Central processor for sensors and commands.",
    detail:
      "Used in model: Main controller board that reads MPU6050/LDR data and sends PWM or direction commands to the motor driver. Significance: Acts as the decision-making brain that runs control logic and keeps all subsystems synchronized.",
  },
  {
    title: "MPU6050",
    tag: "Orientation",
    tagClass: "bg-secondary/10 text-secondary border-secondary/20",
    img: IMG.mpu6050,
    desc: "6-axis IMU for tilt and acceleration.",
    detail:
      "Used in model: Mounted on the satellite frame to measure angular rate and tilt for orientation feedback. Significance: Provides the core attitude data required to detect drift and apply corrective control through the reaction wheel.",
  },
  {
    title: "LDR Sensor",
    tag: "Environmental",
    tagClass: "bg-tertiary/10 text-tertiary border-tertiary/20",
    img: IMG.ldr,
    desc: "Ambient light for sun / shadow sensing.",
    detail:
      "Used in model: Placed on exposed faces to sense sunlight/shadow direction around the model. Significance: Enables light-based pointing behavior and helps validate sun-tracking concepts in the ADCS demonstration.",
  },
  {
    title: "XL6009",
    tag: "Power",
    tagClass: "bg-error/10 text-error border-error/20",
    img: IMG.xl6009,
    desc: "Boost converter for 5V / 12V rails.",
    detail:
      "Used in model: Power stage that boosts battery voltage to stable rails for Arduino and other electronics. Significance: Maintains reliable operation by providing the required voltage headroom when battery voltage changes.",
  },
  {
    title: "TP4056",
    tag: "Power",
    tagClass: "bg-error/10 text-error border-error/20",
    img: IMG.tp4056,
    desc: "Li-ion charge management.",
    detail:
      "Used in model: Charging module between USB input and Li-ion battery pack. Significance: Ensures safe CC/CV charging and battery protection, which is critical for repeatable testing and hardware safety.",
  },
  {
    title: "L293D",
    tag: "Motion",
    tagClass: "bg-secondary/10 text-secondary border-secondary/20",
    img: IMG.l293d,
    desc: "H-bridge motor driver.",
    detail:
      "Used in model: Interface stage between Arduino control pins and the reaction wheel motor. Significance: Supplies bidirectional motor drive with enough current handling, allowing controlled torque generation for attitude correction.",
  },
  {
    title: "Battery",
    tag: "Power",
    tagClass: "bg-error/10 text-error border-error/20",
    img: IMG.battery,
    desc: "Primary energy storage.",
    detail:
      "Used in model: Primary onboard energy source feeding the power-conversion and control chain. Significance: Makes the system portable and mission-like; stable stored energy is essential for uninterrupted ADCS operation.",
  },
] as const;

function HardwareCard({
  index,
  relativeIndex,
  onFrontClick,
  title,
  tag,
  img,
  desc,
  detail,
}: {
  index: number;
  relativeIndex: number;
  onFrontClick: () => void;
  title: string;
  tag: string;
  img: string;
  desc: string;
  detail: string;
}) {
  const rotation = (index % 3) - 1;
  const isFront = relativeIndex === 0;
  const isRear = relativeIndex > 0;
  const visible = relativeIndex >= -1 && relativeIndex <= 1;

  const deckStyle =
    relativeIndex === -1
      ? "translate-x-[-46px] translate-y-1 scale-[0.97] opacity-70 z-10"
      : relativeIndex === 0
        ? "translate-x-0 translate-y-0 scale-100 opacity-100 z-30"
        : relativeIndex === 1
          ? "translate-x-12 translate-y-0 scale-[0.98] opacity-85 z-20"
          : "pointer-events-none opacity-0 z-0";

  const cardTilt =
    relativeIndex === 0 ? "-rotate-[4deg]" : relativeIndex === 1 ? "rotate-[4deg]" : "rotate-[7deg]";

  return (
    <article
      className={`group absolute left-1/2 top-0 h-[450px] w-[320px] -translate-x-1/2 [perspective:1200px] transition-all duration-700 ease-out sm:h-[500px] sm:w-[360px] ${
        visible ? deckStyle : "pointer-events-none opacity-0"
      }`}
      onClick={() => {
        if (isFront) onFrontClick();
      }}
    >
      <div className="absolute inset-0">
        <div
          className={`absolute inset-x-8 bottom-2 top-10 rounded-[28px] border border-primary/15 bg-surface-container/40 blur-[4px] transition-all duration-700 ${
            isFront ? "group-hover:translate-x-2 group-hover:rotate-[6deg]" : ""
          }`}
          style={{ transform: `rotate(${9 + rotation * 2}deg)` }}
        />
        <div
          className={`absolute inset-x-6 bottom-1 top-6 rounded-[30px] border border-primary/20 bg-surface-container-high/55 backdrop-blur-md transition-all duration-700 ${
            isFront ? "group-hover:-translate-x-2 group-hover:-rotate-[5deg]" : ""
          }`}
          style={{ transform: `rotate(${-8 + rotation * 2}deg)` }}
        />
      </div>

      <div
        className={`relative flex h-full flex-col overflow-hidden rounded-[28px] border border-primary/35 bg-[#010816] px-8 py-8 backdrop-blur-[10px] transition-all duration-700 sm:px-10 sm:py-9 ${cardTilt} ${
          isFront ? "cursor-pointer group-hover:-translate-y-1 group-hover:rotate-[-2.5deg]" : ""
        } ${isRear ? "brightness-90 saturate-50 blur-[1.8px]" : ""}`}
        style={{
          boxShadow:
            "inset 0 0 0 1px rgba(58,214,255,0.16), inset 0 0 30px rgba(58,214,255,0.12), 0 10px 32px rgba(2,5,14,0.55)",
          backgroundColor: "#010816",
        }}
      >
        <div className="relative mb-5 flex items-start justify-between gap-4">
          <div>
            <p className="font-label flex items-center gap-2 text-xs uppercase tracking-[0.15em] text-primary/90">
              <span className="material-symbols-outlined text-[17px]">developer_board</span>
              Component Visualizer
            </p>
            <h3 className="font-headline mt-2 text-xl font-semibold tracking-tight text-[#61e4ff]">
              {title}
            </h3>
            <p className="font-body mt-1 text-sm text-white/70">Simulating Momentum Exchange</p>
          </div>
          <span className="font-label rounded-md border border-primary/30 bg-primary/10 px-2.5 py-1 text-[10px] uppercase tracking-[0.14em] text-tertiary">
            {tag}_stable
          </span>
        </div>

        <div className="relative mb-4 flex items-center justify-center">
          <div className="relative h-[248px] w-[248px] rounded-[30px] bg-[radial-gradient(circle,rgba(58,214,255,0.08),transparent_68%)]">
            <div className="absolute left-1/2 top-1/2 h-[200px] w-[200px] -translate-x-1/2 -translate-y-1/2 rounded-[26px] border-8 border-white/[0.06]" />
            <div className="absolute left-1/2 top-1/2 h-[176px] w-[176px] -translate-x-1/2 -translate-y-1/2 overflow-hidden rounded-[22px] border-[3px] border-primary/85 shadow-[0_0_18px_rgba(58,214,255,0.42)]">
              <img src={img} alt={title} className="h-full w-full object-cover opacity-85 saturate-110" />
              <div className="absolute inset-0 bg-[radial-gradient(circle,transparent_42%,rgba(4,11,28,0.68)_100%)]" />
            </div>
          </div>
        </div>

        <p className="font-body text-center text-[16px] leading-[1.8] text-white/86 sm:text-[17px]">{desc}</p>

        <p className="font-body mt-3 line-clamp-2 text-center text-[13px] leading-relaxed text-white/60 sm:text-[14px]">
          {detail}
        </p>

        <div className="mt-4 border-t border-primary/15 pt-3">
          <p className="font-label text-center text-[11px] uppercase tracking-[0.15em] text-primary/80 sm:text-xs">
            {title} | {tag} | ADCS Payload
          </p>
        </div>
      </div>
    </article>
  );
}

export function ComponentsPage() {
  const [activeIndex, setActiveIndex] = useState(0);
  const nextCard = () => setActiveIndex((v) => (v + 1) % HARDWARE.length);
  const getRelativeIndex = (index: number) => {
    const total = HARDWARE.length;
    let diff = index - activeIndex;
    if (diff > total / 2) diff -= total;
    if (diff < -total / 2) diff += total;
    return diff;
  };

  return (
    <div className="overflow-x-hidden bg-surface font-body text-on-surface antialiased">
      <SiteHeader />
      <main className="relative z-10 mx-auto w-full max-w-[1600px] px-4 pb-28 pt-28 sm:px-6 lg:px-10">
        <section className="mb-12">
          <div className="mx-auto flex max-w-5xl flex-col items-center gap-6 text-center">
            <div>
              <span className="font-label mb-3 block text-xs uppercase tracking-[0.2em] text-tertiary sm:text-sm">
                Manifest Library
              </span>
              <h1 className="font-headline text-3xl font-bold tracking-tighter text-on-surface sm:text-4xl md:text-5xl">
                Satellite Core <span className="text-primary-dim">Hardware</span>
              </h1>
            </div>
            <p className="font-body max-w-2xl text-sm leading-relaxed text-on-surface-variant sm:text-base">
              The instrumentation payload of your CanSat. Every sensor and controller is a
              critical data node in our interstellar mission.
            </p>
          </div>
          <div className="mt-8 flex justify-center">
            <Link
              to="/components/assessment"
              className="font-label inline-flex items-center gap-2 rounded-lg border border-secondary/40 bg-secondary/10 px-4 py-2 text-xs uppercase tracking-widest text-secondary transition-colors hover:bg-secondary/20"
            >
              <span className="material-symbols-outlined text-base">quiz</span>
              Hardware assessment
            </Link>
          </div>
        </section>

        <section className="mb-20 mt-4">
          <div className="relative mx-auto h-[470px] w-[360px] -translate-x-25 sm:h-[520px] sm:w-[450px] sm:-translate-x-42">
            <div className="absolute left-1/2 top-[54%] h-20 w-[78%] -translate-x-1/2 -translate-y-1/2 rounded-full bg-primary/10 blur-3xl" />
            <div className="absolute inset-5 rounded-[34px] border border-primary/10 bg-[linear-gradient(120deg,rgba(58,214,255,0.05),transparent_50%)]" />
            {HARDWARE.map((c, index) => {
              const relativeIndex = getRelativeIndex(index);
              return (
                <HardwareCard
                  key={c.title}
                  {...c}
                  index={index}
                  relativeIndex={relativeIndex}
                  onFrontClick={nextCard}
                />
              );
            })}
          </div>
          <p className="mt-8 text-center font-label text-[11px] uppercase tracking-[0.2em] text-white/60">
            Tap card to cycle hardware
          </p>
        </section>

        <section className="mx-auto max-w-3xl rounded-2xl border border-dashed border-white/10 bg-white/[0.02] p-6 text-center">
          <p className="font-body text-sm text-on-surface-variant">
            Every hardware module now follows the same stacked card format to keep the
            component library visually consistent.
          </p>
        </section>
      </main>
      <MobileNav />
    </div>
  );
}
