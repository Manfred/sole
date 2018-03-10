require File.expand_path('../lib/sole/version', __FILE__)

Gem::Specification.new do |spec|
  spec.name = "sole"
  spec.version = Sole::VERSION
  spec.authors = [
    "Manfred Stienstra"
  ]
  spec.email = [
    "manfred@fngtps.com"
  ]
  spec.summary = <<-EOF
  Universally unique identifiers packed with useful metadata meant for use with
  storage solutions.
  EOF
  spec.description = <<-EOF
  Sole helps with generating unique identifiers similar to a UUID or ULID but
  packing in additional metadata to make it useful for storage solutions.
  EOF
  spec.homepage = "https://erm.im/sole"
  spec.license = "MIT"

  spec.extensions << 'ext/extconf.rb'
  spec.files = Dir.glob('lib/**/*') + [
    'LICENSE.txt',
    'README.md',
    'ext/extconf.rb',
    'ext/sole.c'
  ]
  spec.require_paths = ["lib"]

  spec.add_development_dependency "rake", "~> 12.3"
  spec.add_development_dependency "minitest-assert_errors"
end
