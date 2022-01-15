# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## Unreleased

### Fixed
- Removing event from dispatcher on delayed event destroyment

## [2.1.1] - 2022-01-14

### Added
- Mutex that protect vector in EventDispatcher
- Test with unsubscribing before event handler object destroyed

### Fixed
- Crash when object with event handler was unsubscribed before destroyed

## [2.1.0] - 2021-12-23

### Added
- NotifiableObject. Allows to automatically clear events from handlers of destroyed instances
- Some basic tests

### Changed
- Now this library has become shared instead header-only



## [2.0.3] - 2021-10-12

### Changed
- Removed unused #include(s)



## [2.0.2] - 2021-09-28

### Added
- Delegate not equal operator

[2.1.1]: https://github.com/uncellon/hlk-events/releases/tag/v2.1.1
[2.1.0]: https://github.com/uncellon/hlk-events/releases/tag/v2.1.0
[2.0.3]: https://github.com/uncellon/hlk-events/commit/bac687dd4ff71dc8081941fa63b11bbd37866d5a
[2.0.2]: https://github.com/uncellon/hlk-events/commit/230c5e4dc605fea303e7c7e05d79141dac7cb4cc