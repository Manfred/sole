# Sole

Sole helps with generating unique identifiers similar to a UUID or ULID but packing in additional metadata to make it useful for storage solutions.

```ruby
Sole.generate #=> 'VGPCCCCC4FPF97MM'
              #    ^^^^^^^^- seconds since epoch
              #              little endian order
              #              to help sharding
              #            ^^^^^^^^- random bytes
```

Sole identifiers work great as a key for AWS S3 keys because the start of the identifier is highly variable causing objects to be nicely distributed across backend servers.