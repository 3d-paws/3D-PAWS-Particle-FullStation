# Particle Console
[← Back to Top](../README.md)

# Getting Started with Particle.io

Particle.io provides an ecosystem for building connected IoT devices, including powerful hardware, cloud connectivity, and easy firmware development[11][12][7].

## Account Creation and Device Setup

- **Sign up** at [particle.io](https://particle.io) to access the Particle Device Cloud and manage your devices[11].
- **Set up your hardware** (Photon, Argon, Boron, etc.) using the [web setup page](https://setup.particle.io), mobile app, or via the Particle CLI for advanced workflows[11][13].
- **Connect your device** by following step-by-step prompts during setup. Devices typically use USB for initial connection, with Wi-Fi or cellular configuration afterward[14][12].

## Choosing Your Development Environment

- **Particle Web IDE**: Build and flash code to your device entirely in a web browser. Suitable for quick prototypes and beginners[15][16].
- **Particle Workbench**: An extension for Visual Studio Code for local building, project management, and advanced debugging. Supports all Particle platforms[7][17].
- **Particle CLI**: Offers device setup, management, and cloud flashing via terminal. Ideal for scripting and power users[14].

## Flashing Firmware

- Start with the **“Blink” LED example** to validate setup.
- Compile and **flash firmware OTA (over the air)** or via USB directly from either IDE[18][19].
- Use serial monitor/logging tools to view debug output.

## Documentation and Community Resources

- Explore the **Particle Docs** for official hardware/software guides, API references, and troubleshooting[20][10].
- Visit the Particle Community forums, Instructables, or Hackster.io for tutorials and example projects[21][22][19].
- Contribute guides or project documentation in Markdown format for best compatibility with Particle and GitHub[1][2][6].

## Quick Reference Table

| Step                | Tool/Platform          | Reference                      |
|---------------------|-----------------------|--------------------------------|
| Create Account      | Particle Website      | Web, Mobile App                |
| Device Setup        | Web Setup, Mobile App, CLI | setup.particle.io         |
| Programming         | Web IDE, Workbench    | build.particle.io, VS Code     |
| Flash/Test Firmware | OTA, USB              | Web IDE, Workbench, CLI        |
| Documentation       | Particle Docs         | docs.particle.io               |

### [Particle Console Do Action Functions](DoAction.md)
From Particle Console send commands to boards.

### [Particle Web Hooks](WebHooks.md)
Define Webhooks to forward data to external websites like CHORDS.
