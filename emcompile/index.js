import { execSync } from 'child_process';

const emscriptenHelper = (args) => {
  const compileFunctions = args.slice(3)
    .reduce((str, funcName) => str += `, '_${funcName}'`, '');

  const cmd = `em++ ${args[0]} -o ${args[1]} -${args[2]} -s WASM=1 -s  EXTRA_EXPORTED_RUNTIME_METHODS=['ccall','cwrap'] -s EXPORTED_FUNCTIONS="['_main'${compileFunctions}]"`;

  execSync(cmd, { encoding: 'utf-8' });
};

((argv) => {
  try {
    const args = process.argv.slice(2);
    emscriptenHelper(args);
  } catch (err) {
    console.log(err);
  }
})();

// 'EXPORT_ES6=1 -s MODULARIZE=1 -s'