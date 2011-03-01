require File.join(File.dirname(__FILE__), 'spec_helper')

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
  end
end

describe Rygments::Wrapper do
  it "should raise an exception when given an invalid helper path" do
    expect {
      Rygments::Wrapper.new("this_path_does_not_exist")
    }.to raise_error
  end

  subject do
    Rygments::Wrapper.new(Rygments::HELPER_PATH)
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
