# Glaziery

Windowing toolkit and abstraction layer for C++ DirectX games and applications.

Provides concepts like form components, layouts, dialogs, lists, etc.

Engine of [JukeThat, the music jukebox](https://www.jukethat.com).

## Features

Caching support to improve rendering on performance-critical applications like games.

Highly extensible and flexible, hierarchical windowing component architecture.

Strong separation of concerns:

- The component objects contain the state (models).
  Drawing is delegated from the components to the skin.

- The skin defines the layout, colors, bitmaps and the UI behavior of the components.
  Primitive drawing operations are delegated to the platform adapter.

- The platform adapter is the only part of Glaziery containing platform depending code.
  It performs primitive operations.

## Documentation

[API documentation](docs/API/index.html)

## Contact

For more information, please [contact me](https://www.thomasjacob.de/footer/contact).
