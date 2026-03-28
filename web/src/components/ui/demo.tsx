"use client";

import * as React from "react";
import { MoonStar, Sun } from "lucide-react";

import { Avatar, AvatarFallback, AvatarImage } from "@/components/ui/avatar";
import {
  CardTransformed,
  CardsContainer,
  ContainerScroll,
  ReviewStars,
} from "@/components/ui/animated-cards-stack";

const TESTIMONIALS = [
  {
    id: "testimonial-3",
    name: "James S.",
    profession: "Frontend Developer",
    rating: 5,
    description:
      "Their innovative solutions and quick turnaround time made our collaboration incredibly successful. Highly recommended!",
    avatarUrl:
      "https://images.unsplash.com/photo-1500648767791-00dcc994a43e?q=80&w=1974&auto=format&fit=crop",
  },
  {
    id: "testimonial-1",
    name: "Jessica H.",
    profession: "Web Designer",
    rating: 4.5,
    description:
      "The attention to detail and user experience in their work is exceptional. I'm thoroughly impressed with the final product.",
    avatarUrl:
      "https://images.unsplash.com/photo-1494790108377-be9c29b29330?q=80&w=1600&auto=format&fit=crop",
  },
  {
    id: "testimonial-2",
    name: "Lisa M.",
    profession: "UX Designer",
    rating: 5,
    description:
      "Working with them was a game-changer for our project. Their expertise and professionalism exceeded our expectations.",
    avatarUrl:
      "https://images.unsplash.com/photo-1534528741775-53994a69daeb?q=80&w=1600&auto=format&fit=crop",
  },
  {
    id: "testimonial-4",
    name: "Jane D.",
    profession: "UI/UX Designer",
    rating: 4.5,
    description:
      "The quality of work and communication throughout the project was outstanding. They delivered exactly what we needed.",
    avatarUrl:
      "https://images.unsplash.com/photo-1580489944761-15a19d654956?q=80&w=1600&auto=format&fit=crop",
  },
];

const ANIM_IMAGES = [
  "https://images.unsplash.com/photo-1518770660439-4636190af475?q=80&w=2000&auto=format&fit=crop",
  "https://images.unsplash.com/photo-1462331940025-496dfbfc7564?q=80&w=2000&auto=format&fit=crop",
  "https://images.unsplash.com/photo-1523961131990-5ea7c61b2107?q=80&w=2000&auto=format&fit=crop",
  "https://images.unsplash.com/photo-1446776811953-b23d57bd21aa?q=80&w=2000&auto=format&fit=crop",
  "https://images.unsplash.com/photo-1465101162946-4377e57745c3?q=80&w=2000&auto=format&fit=crop",
];

function getSectionClass(isDark: boolean) {
  return isDark ? "bg-destructive text-secondary px-8 py-12" : "bg-accent px-8 py-12";
}

function getReviewStarsClass(isDark: boolean) {
  return isDark ? "text-primary" : "text-teal-500";
}

function getTextClass(isDark: boolean) {
  return isDark ? "text-primary-foreground" : "";
}

function getAvatarClass(isDark: boolean) {
  return isDark ? "!size-12 border border-stone-700" : "!size-12 border border-stone-300";
}

function getCardVariant(isDark: boolean) {
  return isDark ? "dark" : "light";
}

export function TestimonialsVariant({ isDark }: { isDark: boolean }) {
  return (
    <section className={getSectionClass(isDark)}>
      <div>
        <h3 className="text-center text-4xl font-semibold">Testimonials</h3>
        <p className="mx-auto mt-2 max-w-lg text-center text-sm">
          Lorem ipsum dolor sit amet consectetur adipisicing elit. Repellendus consequatur reprehenderit.
        </p>
      </div>
      <ContainerScroll className="container h-[300vh]">
        <div className="sticky left-0 top-0 h-svh w-full py-12">
          <CardsContainer className="mx-auto size-full h-[450px] w-[350px]">
            {TESTIMONIALS.map((testimonial, index) => (
              <CardTransformed
                arrayLength={TESTIMONIALS.length}
                key={testimonial.id}
                variant={getCardVariant(isDark)}
                index={index + 2}
                role="article"
                aria-labelledby={`card-${testimonial.id}-title`}
                aria-describedby={`card-${testimonial.id}-content`}
              >
                <div className="flex flex-col items-center space-y-4 text-center">
                  <ReviewStars className={getReviewStarsClass(isDark)} rating={testimonial.rating} />
                  <div className={`mx-auto w-4/5 text-lg ${getTextClass(isDark)}`}>
                    <blockquote>{testimonial.description}</blockquote>
                  </div>
                </div>
                <div className="flex items-center gap-4">
                  <Avatar className={getAvatarClass(isDark)}>
                    <AvatarImage src={testimonial.avatarUrl} alt={`Portrait of ${testimonial.name}`} />
                    <AvatarFallback>
                      {testimonial.name
                        .split(" ")
                        .map((n) => n[0])
                        .join("")}
                    </AvatarFallback>
                  </Avatar>
                  <div>
                    <span className="block text-lg font-semibold tracking-tight md:text-xl">{testimonial.name}</span>
                    <span className="block text-sm text-muted-foreground">{testimonial.profession}</span>
                  </div>
                </div>
              </CardTransformed>
            ))}
          </CardsContainer>
        </div>
      </ContainerScroll>
    </section>
  );
}

export function AwardsVariant() {
  return (
    <section className="bg-accent px-8 py-12">
      <div>
        <h2 className="text-center text-4xl font-semibold">Recognitions</h2>
        <p className="mx-auto mt-2 max-w-lg text-center text-sm">
          Lorem ipsum dolor sit amet consectetur adipisicing elit. Repellendus consequatur reprehenderit.
        </p>
      </div>
      <ContainerScroll className="container h-[300vh]">
        <div className="sticky left-0 top-0 h-svh w-full py-12">
          <CardsContainer className="mx-auto size-full h-72 w-[440px]">
            <CardTransformed
              className="items-start justify-evenly border-none bg-blue-600/80 text-secondary backdrop-blur-md"
              arrayLength={TESTIMONIALS.length}
              index={1}
            >
              <div className="flex flex-col items-start justify-start space-y-4">
                <div className="flex size-16 items-center justify-center rounded-sm bg-secondary/50 text-2xl">🏆</div>
                <div>
                  <h4 className="text-sm uppercase tracking-wide">Awwwards</h4>
                  <h3 className="text-2xl font-bold">Site of the Day</h3>
                </div>
              </div>
              <p className="text-secondary/80">
                Lorem ipsum dolor sit amet consectetur adipisicing elit. Repellendus consequatur reprehenderit.
              </p>
            </CardTransformed>

            <CardTransformed
              className="items-start justify-evenly border-none bg-orange-600/80 text-secondary backdrop-blur-md"
              arrayLength={TESTIMONIALS.length}
              index={2}
            >
              <div className="flex flex-col items-start justify-start space-y-4">
                <div className="flex size-16 items-center justify-center rounded-sm bg-secondary/50 text-2xl">🚀</div>
                <div>
                  <h4 className="text-sm uppercase tracking-wide">Performance</h4>
                  <h3 className="text-2xl font-bold">100% Performance Score</h3>
                </div>
              </div>
              <p className="text-secondary/80">
                Lorem ipsum dolor sit amet consectetur adipisicing elit. Repellendus consequatur reprehenderit.
              </p>
            </CardTransformed>
            <CardTransformed
              className="items-start justify-evenly border-none bg-cyan-600/80 text-secondary backdrop-blur-md"
              arrayLength={TESTIMONIALS.length}
              index={3}
            >
              <div className="flex flex-col items-start justify-start space-y-4">
                <div className="flex size-16 items-center justify-center rounded-sm bg-secondary/50 text-2xl">🎯</div>
                <div>
                  <h4 className="text-sm uppercase tracking-wide">CSS awards</h4>
                  <h3 className="text-2xl font-bold">Honorable Mention</h3>
                </div>
              </div>
              <p className="text-secondary/80">
                Lorem ipsum dolor sit amet consectetur adipisicing elit. Repellendus consequatur reprehenderit.
              </p>
            </CardTransformed>
            <CardTransformed
              className="items-start justify-evenly border-none bg-violet-600/80 text-secondary backdrop-blur-md"
              arrayLength={TESTIMONIALS.length}
              index={4}
            >
              <div className="flex flex-col items-start justify-start space-y-4">
                <div className="flex size-16 items-center justify-center rounded-sm bg-secondary/50 text-2xl">🎖</div>
                <div>
                  <h4 className="text-sm uppercase tracking-wide">Awwwards</h4>
                  <h4 className="text-2xl font-bold">Most Creative Design</h4>
                </div>
              </div>
              <p className="text-secondary/80">
                Lorem ipsum dolor sit amet consectetur adipisicing elit. Repellendus consequatur reprehenderit.
              </p>
            </CardTransformed>
          </CardsContainer>
        </div>
      </ContainerScroll>
    </section>
  );
}

export function ImagesVariant() {
  return (
    <section className="bg-slate-900 px-8 py-12">
      <div>
        <h2 className="text-center text-4xl font-semibold">Try our AI Fantazomai</h2>
        <p className="mx-auto mt-2 max-w-lg text-center text-sm">
          Lorem ipsum dolor sit amet consectetur adipisicing elit. Repellendus consequatur reprehenderit.
        </p>
      </div>
      <ContainerScroll className="container h-[300vh]">
        <div className="sticky left-0 top-0 h-svh w-full py-12">
          <CardsContainer className="mx-auto size-full h-[420px] w-[320px]">
            {ANIM_IMAGES.map((imageUrl, index) => (
              <CardTransformed
                arrayLength={ANIM_IMAGES.length}
                key={index}
                index={index + 2}
                variant="dark"
                className="overflow-hidden !rounded-sm !p-0"
              >
                <img src={imageUrl} alt="showcase" className="size-full object-cover" />
              </CardTransformed>
            ))}
          </CardsContainer>
        </div>
      </ContainerScroll>
    </section>
  );
}

export function AnimatedCardsStackDemo() {
  const [isDark, setIsDark] = React.useState(false);

  return (
    <div className={isDark ? "dark" : ""}>
      <div className="sticky top-0 z-10 flex justify-end border-b border-border bg-background/80 p-3 backdrop-blur">
        <button
          type="button"
          onClick={() => setIsDark((prev) => !prev)}
          className="inline-flex items-center gap-2 rounded-md border border-border bg-card px-3 py-2 text-sm"
        >
          {isDark ? <MoonStar className="size-4" /> : <Sun className="size-4" />}
          {isDark ? "Dark" : "Light"}
        </button>
      </div>
      <TestimonialsVariant isDark={isDark} />
      <AwardsVariant />
      <ImagesVariant />
    </div>
  );
}
