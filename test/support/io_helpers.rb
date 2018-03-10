module Support
  module IOHelpers
    def capture_stderr
      before = $stderr
      $stderr = captured = StringIO.new
      begin
        yield
      ensure
        $stderr = before
      end
      captured
    end
  end
end
