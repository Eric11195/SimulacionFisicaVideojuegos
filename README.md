# Galactic Armada

3D space shooter built for a university physics simulation course. Core focus: implementing a physics engine layer from scratch on top of NVIDIA PhysX — force generators, particle systems, rigid body dynamics, and spring constraints.

![Architecture Diagram](Diagrama_arquitectura.jpg)

---

## Physics Systems

### Force Generator Framework
Extensible, polymorphic force system. Every force type inherits from `ForceGenerator` and applies per-object via `apply_force(GameObject&)`.

| Generator | Physics |
|---|---|
| `Gravity_ForceGenerator` | Mass-scaled gravitational force |
| `Wind_ForceGenerator` | Aerodynamic drag — uses air density + drag coefficient (Cd) |
| `TorbellinoSencillo` | Vortex with height-bounded area of influence |
| `Spring_ForceGenerator` | Hooke's law — both object-object and point-anchored variants |
| `Floating_ForceGenerator` | Buoyancy based on fluid density and submersion |
| `Variable_ForceGenerator` | Time-varying force via `std::function` lambda — arbitrary curves |

### Particle System
Data-driven particle architecture. Each `ParticleGenerator` takes a config of lambda functions that compute per-particle initial position, direction, velocity, lifetime, color, size, and mass. This makes spawning highly configurable without subclassing for each effect.

Three generator modes:
- `ParticleGenerator` — continuous emission at configurable rate
- `TriggeredParticleGenerator` — one-shot burst on `trigger()` call (used for explosions)
- `ToggleParticleGenerator` — on/off toggle (used for engine thrust)

Particles self-cull by lifetime or area-of-interest predicate (also a configurable lambda).

### Rigid Body Dynamics
Built on `PxRigidDynamic`. Custom `Rigidbody_Object` wrapper exposes torque application, velocity control, and transform synchronization. Static/dynamic variants for scene geometry and active objects.

### Integration
Semi-implicit (symplectic) Euler by default — velocity updated before position, giving better energy conservation than explicit Euler for spring systems. Configurable at compile time via `#define`.

### Spring Projectile (Boleadoras)
Two rigid bodies connected by a spring. `SpringJoinedProjectileLauncher` spawns both particles and attaches an `OBJ_OBJ_Spring_ForceGenerator` between them — real two-body spring constraint simulated through force application each tick.

### Statistical Distributions
`Distributions` namespace wraps `<random>` into reusable samplers: uniform `[0,1]`, normal distribution (three σ presets), and random sign. Used to randomize particle spawn parameters.

---

## Gameplay

Destroy 10 enemy ships to win.

### Weapons
| Weapon | Mechanic |
|---|---|
| Laser (LMB, hold) | Rapid-fire from 4 cannons, one fires per click cycling through all |
| Boleadoras (RMB) | Two projectiles joined by a live spring constraint |
| Missile (Space) | Single slow heavy projectile |

### Controls
| Input | Action |
|---|---|
| Mouse position | Ship orientation — distance from center controls rotation speed |
| W | Thruster — accelerate in look direction |
| A / D | Roll on forward axis |

### Enemy AI
Each `EnemyShip` applies directional thrust toward the player and uses torque to steer — `think_off_torque()` computes corrective torque each tick to face the player. On death: triggers an explosion `TriggeredParticleGenerator`, delays destruction by 3 seconds for the effect to play out.

---

## Architecture

See [architecture diagram](Diagrama_arquitectura.jpg) and the full [project report](Izan_de_Vega_Memoria_Proyecto_Final_físicas.pdf) for detailed system design, torque graphs, and implementation notes.

**Stack:** C++ · NVIDIA PhysX · OpenGL · GLUT

**Key design patterns:** Component hierarchy (`SceneObject → RigidbodyObject → Ship/EnemyShip`), strategy pattern for force generators, descriptor/factory pattern for particle systems.
