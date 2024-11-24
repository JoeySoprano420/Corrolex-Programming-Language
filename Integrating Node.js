const fs = require('fs');
const { exec } = require('child_process');

function compileCorrolex(source) {
    fs.writeFileSync('source.cor', source);
    exec('gcc -o corrolex_compiler source.cor', (error, stdout, stderr) => {
        if (error) {
            console.error(`Error compiling Corrolex: ${error}`);
            return;
        }
        console.log(stdout);
    });
}
