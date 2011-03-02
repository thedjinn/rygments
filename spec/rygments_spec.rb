require File.join(File.dirname(__FILE__), 'spec_helper')

describe Rygments::Wrapper do
  subject do
    # We don't want to recreate wrappers because reinitializing Python
    # fails on OSX
    Rygments.wrapper
  end

  describe "#highlight_string" do
    it "should return a string" do
      subject.highlight_string("puts 'Hello World!'", "ruby", "html").should be_a(String)
    end

    it "should raise an exception when given an invalid formatter" do
      expect {
        subject.highlight_string("puts 'Hello World!", "ruby", "awesome_nonexistant_formatter")
      }.to raise_error
    end

    it "should raise an exception when given an invalid lexer" do
      expect {
        subject.highlight_string("puts 'Hello World!", "awesome_nonexistant_lexer", "html")
      }.to raise_error
    end
  end

  describe "#highlight_file" do
    it "should return a string" do
      subject.highlight_file(__FILE__, "ruby", "html").should be_a(String)
    end

    it "should raise an exception when given a nonexisting filename" do
      expect {
        subject.highlight_file("this_file_does_not_exist", "ruby", "html")
      }.to raise_error
    end

    it "should raise an exception when given an invalid formatter" do
      expect {
        subject.highlight_file(__FILE__, "ruby", "awesome_nonexistant_formatter")
      }.to raise_error
    end

    it "should raise an exception when given an invalid lexer" do
      expect {
        subject.highlight_file(__FILE__, "awesome_nonexistant_lexer", "html")
      }.to raise_error
    end
  end
end

describe Rygments do
  describe "#highlight_string" do
    it "should return a string" do
      Rygments.highlight_string("puts 'Hello World!'", "ruby", "html").should be_a(String)
    end

    it "should raise an exception when given an invalid formatter" do
      expect {
        Rygments.highlight_string("puts 'Hello World!", "ruby", "awesome_nonexistant_formatter")
      }.to raise_error
    end

    it "should raise an exception when given an invalid lexer" do
      expect {
        Rygments.highlight_string("puts 'Hello World!", "awesome_nonexistant_lexer", "html")
      }.to raise_error
    end

    it "should raise an exception when the first argument is not a string" do
      expect {
        Rygments.highlight_string(:no_string, "ruby", "html")
      }.to raise_error
    end

    it "should raise an exception when the second argument is not a string" do
      expect {
        Rygments.highlight_string("puts 'Hello World!", :no_string, "html")
      }.to raise_error
    end

    it "should raise an exception when the third argument is not a string" do
      expect {
        Rygments.highlight_string("puts 'Hello World!", "ruby", :no_string)
      }.to raise_error
    end
  end
  
  describe "#highlight_file" do
    it "should return a string" do
      Rygments.highlight_file(__FILE__, "ruby", "html").should be_a(String)
    end

    it "should raise an exception when given an invalid formatter" do
      expect {
        Rygments.highlight_file(__FILE__, "ruby", "awesome_nonexistant_formatter")
      }.to raise_error
    end

    it "should raise an exception when given an invalid lexer" do
      expect {
        Rygments.highlight_file(__FILE__, "awesome_nonexistant_lexer", "html")
      }.to raise_error
    end

    it "should raise an exception when the first argument is not a string" do
      expect {
        Rygments.highlight_file(:no_string, "ruby", "html")
      }.to raise_error
    end

    it "should raise an exception when the second argument is not a string" do
      expect {
        Rygments.highlight_file(__FILE__, :no_string, "html")
      }.to raise_error
    end

    it "should raise an exception when the third argument is not a string" do
      expect {
        Rygments.highlight_file(__FILE__, "ruby", :no_string)
      }.to raise_error
    end
  end
end
