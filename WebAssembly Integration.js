(async () => {
    const wasm = await WebAssembly.instantiateStreaming(fetch('corrolex.wasm'));
    const { execute } = wasm.instance.exports;

    // Execute the bytecode directly from the browser
    execute(new Uint8Array([/* bytecode here */]));
})();
