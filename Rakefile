require "bundler/gem_tasks"
require "rake/testtask"

task default: :test

Rake::TestTask.new(:test) do |t|
  t.test_files = FileList['test/**/*_test.rb']
  t.verbose = true
end

task :compile => "extconf:compile"

namespace :extconf do
  task :makefile do
    Dir.chdir('ext') do
      ruby "extconf.rb"
    end
  end

  task make: :makefile do
    Dir.chdir('ext') do
      sh(RUBY_PLATFORM =~ /win32/ ? 'nmake' : 'make') do |ok, res|
        if !ok
          require "fileutils"
          FileUtils.rm_rf(Dir.glob('*.{so,o,dll,bundle}'))
        end
      end
    end
  end

  desc "Compile the Ruby extension"
  task compile: :make do
    if Dir.glob("ext/*.{o,so,dll}").length == 0
      $stderr.puts("Failed to build ext.")
      exit(1)
    end
  end
end
