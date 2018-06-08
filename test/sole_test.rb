# frozen_string_literal: true

require_relative "preamble"
require 'base64'

class SoleTest < Minitest::Test
  def test_generates
    string = Sole.generate
    refute_equal '', string
    assert_equal 16, string.length
  end

  def test_packs
    string = Sole.pack(43, 'Bob')
    refute_equal '', string
    assert_equal 28, string.length

    string = Sole.pack(43, 'Café')
    refute_equal '', string
    assert_equal 32, string.length
  end

  def test_parses
    parts = Sole.unpack("4GC4FCCCRVF9RG4PHJHHF7JGP9PG")
    assert_equal 2, parts.length
    assert_equal '43', parts[0]
    assert_equal 'Bob', parts[1]

    parts = Sole.unpack("4HC4FCCCMGRWWPQ9HJHHF7JHPFPP2HWV")
    assert_equal 2, parts.length
    assert_equal '43', parts[0]
    assert_equal 'Café', parts[1]
  end

  def test_roundtrip
    packed = Sole.pack(43, 'Café')
    assert_equal 32, packed.length
    unpacked = Sole.unpack(packed)
    assert_equal 2, unpacked.length
    assert_equal '43', unpacked[0]
    assert_equal 'Café', unpacked[1]
  end
end
