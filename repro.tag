<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile>
  <compound kind="file">
    <name>future.h</name>
    <path>/home/mike/workspace/repro/include/reprocpp/</path>
    <filename>future_8h</filename>
    <class kind="class">repro::FutureMixin</class>
    <class kind="class">repro::Future</class>
  </compound>
  <compound kind="file">
    <name>promise.h</name>
    <path>/home/mike/workspace/repro/include/reprocpp/</path>
    <filename>promise_8h</filename>
    <includes id="future_8h" name="future.h" local="yes" imported="no">reprocpp/future.h</includes>
    <class kind="class">repro::PromiseMixin</class>
    <class kind="class">repro::Promise</class>
    <member kind="function">
      <type>Promise&lt; Args... &gt;</type>
      <name>promise</name>
      <anchorfile>promise_8h.html</anchorfile>
      <anchor>ac5aa6da8e9afe3c9d4fe55491f1eeeba</anchor>
      <arglist>() noexcept</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>repro::Ex</name>
    <filename>classrepro_1_1Ex.html</filename>
    <member kind="function">
      <type></type>
      <name>Ex</name>
      <anchorfile>classrepro_1_1Ex.html</anchorfile>
      <anchor>a9eaaf09719c53be9cfa6fb7c59ea374f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Ex</name>
      <anchorfile>classrepro_1_1Ex.html</anchorfile>
      <anchor>a3c589da4aff16e44def59566a743fb11</anchor>
      <arglist>(const std::string &amp;m)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>what</name>
      <anchorfile>classrepro_1_1Ex.html</anchorfile>
      <anchor>a6f455fcc69cad850efe606c086e9ccdc</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual std::exception_ptr</type>
      <name>make_exception_ptr</name>
      <anchorfile>classrepro_1_1Ex.html</anchorfile>
      <anchor>aee29dcbeed17d4e1fa9a0b03b6d03148</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="variable">
      <type>std::string</type>
      <name>msg</name>
      <anchorfile>classrepro_1_1Ex.html</anchorfile>
      <anchor>a02c71ac66a4757573ba575f36fdfb547</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>repro::Future</name>
    <filename>classrepro_1_1Future.html</filename>
    <templarg>Args</templarg>
    <base>FutureMixin&lt; Args... &gt;</base>
  </compound>
  <compound kind="class">
    <name>repro::FutureMixin</name>
    <filename>classrepro_1_1FutureMixin.html</filename>
    <templarg>Args</templarg>
    <member kind="function">
      <type>Future&lt; Args... &gt; &amp;</type>
      <name>then</name>
      <anchorfile>classrepro_1_1FutureMixin.html</anchorfile>
      <anchor>a33606d2e56d8d79609c2cdf4bbc8db98</anchor>
      <arglist>(F f) noexcept</arglist>
    </member>
    <member kind="function">
      <type>auto</type>
      <name>then</name>
      <anchorfile>classrepro_1_1FutureMixin.html</anchorfile>
      <anchor>acd9fe642ad5bfa386fe1466070e2cf99</anchor>
      <arglist>(F f) noexcept -&gt; typename std::result_of&lt; F(Args ...)&gt;::type</arglist>
    </member>
    <member kind="function">
      <type>Future&lt; Args... &gt; &amp;</type>
      <name>otherwise</name>
      <anchorfile>classrepro_1_1FutureMixin.html</anchorfile>
      <anchor>ac1e77b0676d90992873482f59424db58</anchor>
      <arglist>(E e) noexcept</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>FutureMixin&lt; Args... &gt;</name>
    <filename>classrepro_1_1FutureMixin.html</filename>
    <member kind="function">
      <type>Future&lt; Args... &gt; &amp;</type>
      <name>then</name>
      <anchorfile>classrepro_1_1FutureMixin.html</anchorfile>
      <anchor>a33606d2e56d8d79609c2cdf4bbc8db98</anchor>
      <arglist>(F f) noexcept</arglist>
    </member>
    <member kind="function">
      <type>auto</type>
      <name>then</name>
      <anchorfile>classrepro_1_1FutureMixin.html</anchorfile>
      <anchor>acd9fe642ad5bfa386fe1466070e2cf99</anchor>
      <arglist>(F f) noexcept -&gt; typename std::result_of&lt; F(Args ...)&gt;::type</arglist>
    </member>
    <member kind="function">
      <type>Future&lt; Args... &gt; &amp;</type>
      <name>otherwise</name>
      <anchorfile>classrepro_1_1FutureMixin.html</anchorfile>
      <anchor>ac1e77b0676d90992873482f59424db58</anchor>
      <arglist>(E e) noexcept</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>FutureMixin&lt; T &amp;&gt;</name>
    <filename>classrepro_1_1FutureMixin.html</filename>
    <member kind="function">
      <type>Future&lt; Args... &gt; &amp;</type>
      <name>then</name>
      <anchorfile>classrepro_1_1FutureMixin.html</anchorfile>
      <anchor>a33606d2e56d8d79609c2cdf4bbc8db98</anchor>
      <arglist>(F f) noexcept</arglist>
    </member>
    <member kind="function">
      <type>auto</type>
      <name>then</name>
      <anchorfile>classrepro_1_1FutureMixin.html</anchorfile>
      <anchor>acd9fe642ad5bfa386fe1466070e2cf99</anchor>
      <arglist>(F f) noexcept -&gt; typename std::result_of&lt; F(Args ...)&gt;::type</arglist>
    </member>
    <member kind="function">
      <type>Future&lt; Args... &gt; &amp;</type>
      <name>otherwise</name>
      <anchorfile>classrepro_1_1FutureMixin.html</anchorfile>
      <anchor>ac1e77b0676d90992873482f59424db58</anchor>
      <arglist>(E e) noexcept</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>FutureMixin&lt; T &gt;</name>
    <filename>classrepro_1_1FutureMixin.html</filename>
    <member kind="function">
      <type>Future&lt; Args... &gt; &amp;</type>
      <name>then</name>
      <anchorfile>classrepro_1_1FutureMixin.html</anchorfile>
      <anchor>a33606d2e56d8d79609c2cdf4bbc8db98</anchor>
      <arglist>(F f) noexcept</arglist>
    </member>
    <member kind="function">
      <type>auto</type>
      <name>then</name>
      <anchorfile>classrepro_1_1FutureMixin.html</anchorfile>
      <anchor>acd9fe642ad5bfa386fe1466070e2cf99</anchor>
      <arglist>(F f) noexcept -&gt; typename std::result_of&lt; F(Args ...)&gt;::type</arglist>
    </member>
    <member kind="function">
      <type>Future&lt; Args... &gt; &amp;</type>
      <name>otherwise</name>
      <anchorfile>classrepro_1_1FutureMixin.html</anchorfile>
      <anchor>ac1e77b0676d90992873482f59424db58</anchor>
      <arglist>(E e) noexcept</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>FutureMixin&lt;&gt;</name>
    <filename>classrepro_1_1FutureMixin.html</filename>
    <member kind="function">
      <type>Future&lt; Args... &gt; &amp;</type>
      <name>then</name>
      <anchorfile>classrepro_1_1FutureMixin.html</anchorfile>
      <anchor>a33606d2e56d8d79609c2cdf4bbc8db98</anchor>
      <arglist>(F f) noexcept</arglist>
    </member>
    <member kind="function">
      <type>auto</type>
      <name>then</name>
      <anchorfile>classrepro_1_1FutureMixin.html</anchorfile>
      <anchor>acd9fe642ad5bfa386fe1466070e2cf99</anchor>
      <arglist>(F f) noexcept -&gt; typename std::result_of&lt; F(Args ...)&gt;::type</arglist>
    </member>
    <member kind="function">
      <type>Future&lt; Args... &gt; &amp;</type>
      <name>otherwise</name>
      <anchorfile>classrepro_1_1FutureMixin.html</anchorfile>
      <anchor>ac1e77b0676d90992873482f59424db58</anchor>
      <arglist>(E e) noexcept</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>repro::Promise</name>
    <filename>classrepro_1_1Promise.html</filename>
    <templarg>Args</templarg>
    <base>PromiseMixin&lt; Args... &gt;</base>
  </compound>
  <compound kind="class">
    <name>repro::PromiseMixin</name>
    <filename>classrepro_1_1PromiseMixin.html</filename>
    <templarg>Args</templarg>
    <member kind="function">
      <type>Future&lt; Args... &gt;</type>
      <name>future</name>
      <anchorfile>classrepro_1_1PromiseMixin.html</anchorfile>
      <anchor>a47ce15334ede44f3586e011600872197</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>resolve</name>
      <anchorfile>classrepro_1_1PromiseMixin.html</anchorfile>
      <anchor>a73f2f9653c56ff0a69351e4f4cb5adee</anchor>
      <arglist>(VArgs &amp;&amp;... args) const noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>resolve</name>
      <anchorfile>classrepro_1_1PromiseMixin.html</anchorfile>
      <anchor>a07c42d332ee796c5173f9ae165e98ebc</anchor>
      <arglist>(Future&lt; Args... &gt; &amp;f) const noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>reject</name>
      <anchorfile>classrepro_1_1PromiseMixin.html</anchorfile>
      <anchor>a2bdc0dabf1040fa4f9df996ec5526c9d</anchor>
      <arglist>(const E &amp;e) const noexcept</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>PromiseMixin&lt; Args... &gt;</name>
    <filename>classrepro_1_1PromiseMixin.html</filename>
    <member kind="function">
      <type>Future&lt; Args... &gt;</type>
      <name>future</name>
      <anchorfile>classrepro_1_1PromiseMixin.html</anchorfile>
      <anchor>a47ce15334ede44f3586e011600872197</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>resolve</name>
      <anchorfile>classrepro_1_1PromiseMixin.html</anchorfile>
      <anchor>a73f2f9653c56ff0a69351e4f4cb5adee</anchor>
      <arglist>(VArgs &amp;&amp;... args) const noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>resolve</name>
      <anchorfile>classrepro_1_1PromiseMixin.html</anchorfile>
      <anchor>a07c42d332ee796c5173f9ae165e98ebc</anchor>
      <arglist>(Future&lt; Args... &gt; &amp;f) const noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>reject</name>
      <anchorfile>classrepro_1_1PromiseMixin.html</anchorfile>
      <anchor>a2bdc0dabf1040fa4f9df996ec5526c9d</anchor>
      <arglist>(const E &amp;e) const noexcept</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>repro::ReproEx</name>
    <filename>classrepro_1_1ReproEx.html</filename>
    <templarg></templarg>
    <base>repro::Ex</base>
    <member kind="function" virtualness="virtual">
      <type>virtual std::exception_ptr</type>
      <name>make_exception_ptr</name>
      <anchorfile>classrepro_1_1ReproEx.html</anchorfile>
      <anchor>a19599ac72394837d918bb9f83c64fdf6</anchor>
      <arglist>() const</arglist>
    </member>
  </compound>
  <compound kind="page">
    <name>md_README</name>
    <title>repro</title>
    <filename>md_README</filename>
  </compound>
</tagfile>
