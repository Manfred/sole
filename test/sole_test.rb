# frozen_string_literal: true

require_relative "preamble"

class SoleTest < Minitest::Test
  def test_generates_bytes
    assert_nil Sole.bytes
  end
end
