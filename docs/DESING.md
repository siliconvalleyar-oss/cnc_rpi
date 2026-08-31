# Design Document

## System Overview

The CNC Controller is an embedded application for Raspberry Pi that interprets G-code and controls up to 5 stepper motors via Pololu drivers.

## Design Principles

1. **Simplicity**: Keep code readable and maintainable
2. **Safety**: Hardware limits and emergency stop support
3. **Portability**: Support ARM32 and ARM64
4. **Extensibility**: Modular architecture for new features

## Component Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│                         main.cpp                                │
│                    (Entry Point)                                │
└────────────────────────────┬────────────────────────────────────┘
                             │
                             ▼
┌─────────────────────────────────────────────────────────────────┐
│                      Device::Device_t                           │
│  ┌─────────────┐  ┌──────────────┐  ┌────────────────────┐    │
│  │ GCodeParser │  │ MotorControl │  │ DisplayManager     │    │
│  └─────────────┘  └──────────────┘  └────────────────────┘    │
│  ┌─────────────┐  ┌──────────────┐                             │
│  │ EndstopMon  │  │ Interpolator │                             │
│  └─────────────┘  └──────────────┘                             │
└─────────────────────────────────────────────────────────────────┘
                             │
                             ▼
┌─────────────────────────────────────────────────────────────────┐
│                        bcm2835 Library                          │
│                    (Hardware Abstraction)                       │
└─────────────────────────────────────────────────────────────────┘
```

## State Machine

```
                    ┌──────────┐
                    │  IDLE    │
                    └────┬─────┘
                         │ init()
                         ▼
                    ┌──────────┐
         ┌─────────│ READY    │─────────┐
         │         └────┬─────┘         │
         │              │               │
         │         command()            │
         │              │               │
         │              ▼               │
         │         ┌──────────┐         │
         │         │ MOVING   │         │
         │         └────┬─────┘         │
         │              │               │
         │         done │               │
         │              │               │
         ▼              ▼               ▼
    ┌─────────┐   ┌──────────┐   ┌──────────┐
    │  ERROR  │◄──│ HOMING   │   │ SHUTDOWN │
    └─────────┘   └──────────┘   └──────────┘
```

## Memory Management

- All dynamic allocation via `std::unique_ptr`
- No raw `new/delete`
- RAII for hardware resources
- Stack allocation preferred for small objects

## Error Handling

- Return codes for hardware errors
- Exception-free design for real-time constraints
- Graceful degradation on non-critical failures
