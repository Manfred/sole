# frozen_string_literal: true

require_relative "preamble"
require 'base64'

class SoleTest < Minitest::Test
  def test_generates_bytes
    bytes = Sole.generate
    refute_equal '', bytes
    assert_equal 16, bytes.length
  end
end
